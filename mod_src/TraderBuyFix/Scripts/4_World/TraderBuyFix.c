/**
 * TraderBuyFix - BAE-Z
 *
 * DayZ 1.29 broke item delivery in Dr Jones Trader 1.9: the vanilla
 * HumanInventory.CreateInInventory() now creates flipped cargo entries and
 * silently falls back to CreateInHands(), so bought items get paid for but
 * never show up in the inventory or the vicinity.
 *
 * This overrides the Trader's CreateItemInInventory() (server side). The
 * stacking/merge logic is kept as-is; only the spawn step changes:
 *   1. create the item server-local (ECE_LOCAL) at the player's feet
 *   2. move it into cargo/attachment (rotated fit included), else empty hands,
 *      else leave it on the ground next to the player
 *   3. SetSynchDirty() + RemoteObjectCreate() so the client actually gets it
 *   4. apply the configured quantity / ammo count and sync again
 * Technique taken from Workshop 3704049029 "Trader_FIX" (GloryStar). That mod
 * cannot be loaded here as-is: it also mods ItemBase and Ammunition_Base, which
 * DurableGear / AmmoStacks (both -serverMod) already mod, and two server mods
 * on one class deadlock the script compiler.
 */
modded class DayZPlayerImplement
{
	override bool CreateItemInInventory(PlayerBase player, string itemType, int amount)
	{
		array<EntityAI> itemsArray = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
		string itemLower = itemType;
		itemLower.ToLower();

		int currentAmount = amount;
		ItemBase item;
		Ammunition_Base ammoItem;
		bool hasSomeQuant = (TR_Helper.ItemHasCount(itemType) || TR_Helper.ItemHasQuantity(itemType)) && !TR_Helper.HasQuantityBar(itemType) && amount >= 0;
		int itemHasSpawnedOrStacked = 0;

		// autostacking into existing stacks (unchanged from Trader 1.9)
		if (hasSomeQuant)
		{
			for (int i = 0; i < itemsArray.Count(); i++)
			{
				if (currentAmount <= 0)
					break;
				Class.CastTo(item, itemsArray.Get(i));
				string itemPlayerClassname = "";
				if (item)
				{
					if (item.IsRuined())
						continue;

					itemPlayerClassname = item.GetType();
					itemPlayerClassname.ToLower();
					if (itemLower == itemPlayerClassname && !item.IsFullQuantity() && !item.IsMagazine())
					{
						currentAmount = item.AddQuantityTR(currentAmount);
						item.SetSynchDirty();
						itemDisplayNameClient = item.GetDisplayName();
						itemHasSpawnedOrStacked++;
					}
				}

				Class.CastTo(ammoItem, itemsArray.Get(i));
				if (ammoItem)
				{
					if (ammoItem.IsRuined())
						continue;
					itemPlayerClassname = ammoItem.GetType();
					itemPlayerClassname.ToLower();
					if (itemLower == itemPlayerClassname && ammoItem.IsAmmoPile())
					{
						currentAmount = ammoItem.AddQuantityTR(currentAmount);
						ammoItem.SetSynchDirty();
						itemDisplayNameClient = ammoItem.GetDisplayName();
						itemHasSpawnedOrStacked++;
					}
				}
			}
		}
		else
		{
			currentAmount = 1;
		}

		if (itemHasSpawnedOrStacked > 0 && !itemType.Contains("Ruble"))
			TraderMessage.PlayerWhite("#tm_some" + " " + itemDisplayNameClient + "\n" + "#tm_added_to_inventory", PlayerBase.Cast(this));

		// any leftover or new stacks
		if (currentAmount > 0 || !hasSomeQuant)
		{
			InventoryLocationType foundLocType;
			EntityAI newItem = TraderBuyFix_SpawnForPlayer(itemType, amount, currentAmount, hasSomeQuant, foundLocType);
			if (!newItem)
			{
				Error("[TraderBuyFix] Failed to spawn entity " + itemType + " ! Make sure the classname exists and item can be spawned");
				return false;
			}

			if (foundLocType == InventoryLocationType.GROUND)
				TraderMessage.PlayerWhite(newItem.GetDisplayName() + "\n" + "#tm_was_placed_on_ground", PlayerBase.Cast(this));
			else
				TraderMessage.PlayerWhite(newItem.GetDisplayName() + "\n" + "#tm_added_to_inventory", PlayerBase.Cast(this));
		}

		UpdateInventoryMenu();
		return true;
	}

	EntityAI TraderBuyFix_SpawnForPlayer(string itemType, int amount, int currentAmount, bool hasSomeQuant, out InventoryLocationType locType)
	{
		locType = InventoryLocationType.UNKNOWN;

		// Server-local object first. On 1.29 an item that is created networked and then
		// moved into the inventory never shows up on the client. Creating it ECE_LOCAL,
		// moving it, and only then calling RemoteObjectCreate() is what makes it visible
		// (same approach as Workshop 3704049029 "Trader_FIX").
		EntityAI newItem = EntityAI.Cast(GetGame().CreateObjectEx(itemType, GetPosition(), ECE_LOCAL));
		if (!newItem)
			return null;

		InventoryLocation src = new InventoryLocation();
		InventoryLocation dst = new InventoryLocation();
		if (!newItem.GetInventory().GetCurrentInventoryLocation(src))
		{
			GetGame().ObjectDelete(newItem);
			return null;
		}

		// 1. cargo / attachment slot (FindFreeLocationFor also tries the rotated fit)
		if (GetInventory().FindFreeLocationFor(newItem, FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, dst))
		{
			if (GameInventory.LocationSyncMoveEntity(src, dst))
				locType = dst.GetType();
		}
		// 2. empty hands
		else if (!GetHumanInventory().GetEntityInHands() && GetHumanInventory().CanAddEntityInHands(newItem))
		{
			dst.SetHands(this, newItem);
			if (GameInventory.LocationSyncMoveEntity(src, dst))
				locType = InventoryLocationType.HANDS;
		}
		// 3. stays on the ground at the player's feet
		if (locType == InventoryLocationType.UNKNOWN)
			locType = InventoryLocationType.GROUND;

		newItem.SetSynchDirty();
		SetSynchDirty();
		GetGame().RemoteObjectCreate(newItem);

		// quantity / ammo after the item is networked, then force a sync
		Magazine newMagItem = Magazine.Cast(newItem);
		Ammunition_Base newAmmoItem = Ammunition_Base.Cast(newItem);
		if (newMagItem && !newAmmoItem)
		{
			newMagItem.ServerSetAmmoCount(amount);
		}
		else if (hasSomeQuant)
		{
			if (newAmmoItem)
			{
				newAmmoItem.SetQuantityTR(currentAmount);
			}
			else
			{
				ItemBase newItemBase;
				if (Class.CastTo(newItemBase, newItem))
					newItemBase.SetQuantityTR(currentAmount);
			}
		}
		newItem.SetSynchDirty();

		return newItem;
	}
}
