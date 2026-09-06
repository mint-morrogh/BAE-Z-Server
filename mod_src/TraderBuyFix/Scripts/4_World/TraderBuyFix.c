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
 *   1. create the item at the player's feet
 *   2. apply the configured quantity / ammo count
 *   3. move it into the inventory via ServerTakeEntityToInventory(), the
 *      normal synced server-side take path (handles rotation in 1.29)
 *   4. if it does not fit anywhere, it stays on the ground next to the player
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

		EntityAI newItem = EntityAI.Cast(GetGame().CreateObjectEx(itemType, GetPosition(), ECE_PLACE_ON_SURFACE));
		if (!newItem)
			return null;

		// quantity first so the inventory move never has to split a stack
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

		if (ServerTakeEntityToInventory(FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, newItem))
		{
			InventoryLocation il = new InventoryLocation();
			if (newItem.GetInventory().GetCurrentInventoryLocation(il))
				locType = il.GetType();
			else
				locType = InventoryLocationType.CARGO;
			return newItem;
		}

		locType = InventoryLocationType.GROUND;
		return newItem;
	}
}
