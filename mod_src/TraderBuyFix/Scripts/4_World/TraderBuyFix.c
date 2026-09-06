/**
 * TraderBuyFix - BAE-Z
 *
 * Dr Jones Trader 1.9 on DayZ 1.29: purchases are charged but new items never
 * show up for the player (neither in the inventory nor on the ground). Only
 * top-ups of an existing stack (rose hips, bandages, roubles) work, because
 * those never create a new entity.
 *
 * v1 (networked CreateObjectEx + ServerTakeEntityToInventory) and
 * v2 (ECE_LOCAL + LocationSyncMoveEntity + RemoteObjectCreate, the Trader_FIX
 * 3704049029 technique) both failed in-game.
 *
 * v4 (the actual root cause): Trader's TR_Helper.GetItemMaxQuantity is a `bool`
 * function returning -1 for classes without count/varQuantityMax. 1.29 evaluates
 * that as true, so every non-stackable item became "stackable with amount 0" and
 * CreateItemInInventory skipped the spawn after charging the player. We resolve
 * the max quantity as an int ourselves and spawn one item when there is nothing
 * to stack.
 *
 * v3 uses the path DayZ 1.29 itself uses when it spawns an item into a player
 * (PlayerBase.CreateInInventory -> SpawnItemOnLocation): find a free location
 * for the class name, then GameInventory.LocationCreateEntity() so the engine
 * creates the entity networked, directly at that location. Fallbacks: empty
 * hands, then the ground at the player's feet.
 *
 * Every step is logged with a [TraderBuyFix] prefix to the server script log,
 * and the item is re-checked 3 s later (still exists? where? network id?) so a
 * failed test tells us whether the item vanished server-side or never reached
 * the client.
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
		// Trader 1.9 bug exposed by DayZ 1.29: TR_Helper.GetItemMaxQuantity is declared
		// `bool` but returns -1 when the class has no count/varQuantityMax. 1.29 turns that
		// -1 into `true`, so clothing, cans, etc. are treated as stackables with amount 0
		// (the `*` in TraderConfig resolves to 0 for them) and nothing is ever spawned.
		// Resolve the max quantity as a real int and only stack when there is something to stack.
		int maxQuant = TraderBuyFix_MaxQuantity(itemType);
		bool hasSomeQuant = maxQuant > 0 && !TR_Helper.HasQuantityBar(itemType) && amount > 0;
		int itemHasSpawnedOrStacked = 0;

		TraderBuyFix_Log("buy " + itemType + " amount=" + amount + " maxQuant=" + maxQuant + " quantityBar=" + TR_Helper.HasQuantityBar(itemType) + " hasSomeQuant=" + hasSomeQuant + " inventoryItems=" + itemsArray.Count());

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
						TraderBuyFix_Log("stacked into existing " + itemType + ", leftover=" + currentAmount);
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
						TraderBuyFix_Log("stacked into existing ammo pile " + itemType + ", leftover=" + currentAmount);
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
				TraderBuyFix_Log("FAILED to spawn " + itemType + " anywhere");
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

	// CfgMagazines <class> count, else CfgVehicles <class> varQuantityMax, else 0
	int TraderBuyFix_MaxQuantity(string itemType)
	{
		string path = CFG_MAGAZINESPATH + " " + itemType + " count";
		if (GetGame().ConfigIsExisting(path))
			return GetGame().ConfigGetInt(path);
		path = CFG_VEHICLESPATH + " " + itemType + " varQuantityMax";
		if (GetGame().ConfigIsExisting(path))
			return GetGame().ConfigGetInt(path);
		return 0;
	}

	EntityAI TraderBuyFix_SpawnForPlayer(string itemType, int amount, int currentAmount, bool hasSomeQuant, out InventoryLocationType locType)
	{
		locType = InventoryLocationType.UNKNOWN;
		EntityAI newItem;
		string how = "";
		InventoryLocation loc = new InventoryLocation();

		// 1. cargo / attachment slot: the engine picks the location for this class name and
		//    creates the item networked, directly there (vanilla 1.29 PlayerBase.CreateInInventory path)
		if (GetInventory().FindFirstFreeLocationForNewEntity(itemType, FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, loc))
		{
			TraderBuyFix_Log("free location for " + itemType + ": " + InventoryLocation.DumpToStringNullSafe(loc));
			newItem = GameInventory.LocationCreateEntity(loc, itemType, ECE_IN_INVENTORY, RF_DEFAULT);
			how = "LocationCreateEntity(cargo/attachment)";
			if (!newItem)
				TraderBuyFix_Log("LocationCreateEntity returned null for " + itemType);
		}
		else
		{
			TraderBuyFix_Log("no free cargo/attachment location for " + itemType);
		}

		// 2. empty hands
		if (!newItem && !GetHumanInventory().GetEntityInHands())
		{
			loc = new InventoryLocation();
			if (GetInventory().FindFirstFreeLocationForNewEntity(itemType, FindInventoryLocationType.HANDS, loc))
			{
				newItem = GameInventory.LocationCreateEntity(loc, itemType, ECE_IN_INVENTORY, RF_DEFAULT);
				how = "LocationCreateEntity(hands)";
				if (!newItem)
					TraderBuyFix_Log("LocationCreateEntity(hands) returned null for " + itemType);
			}
		}

		// 3. ground at the player's feet
		if (!newItem)
		{
			vector pos = GetPosition() + GetDirection() * 0.5;
			newItem = EntityAI.Cast(GetGame().CreateObjectEx(itemType, pos, ECE_PLACE_ON_SURFACE));
			how = "CreateObjectEx(ground)";
		}

		if (!newItem)
			return null;

		InventoryLocation cur = new InventoryLocation();
		if (newItem.GetInventory().GetCurrentInventoryLocation(cur))
			locType = cur.GetType();
		else
			locType = InventoryLocationType.GROUND;

		// quantity / ammo
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

		TraderBuyFix_Log("spawned " + itemType + " via " + how + " -> " + TraderBuyFix_Describe(newItem));
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(TraderBuyFix_Recheck, 3000, false, newItem, itemType);

		return newItem;
	}

	// 3 s after the purchase: does the item still exist server-side, where is it, is it networked,
	// and how many of that type does the player's inventory tree hold now.
	void TraderBuyFix_Recheck(EntityAI newItem, string itemType)
	{
		if (!newItem)
		{
			TraderBuyFix_Log("RECHECK " + itemType + ": item is GONE (deleted server-side within 3 s)");
			return;
		}

		array<EntityAI> itemsArray = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
		int count = 0;
		string lower = itemType;
		lower.ToLower();
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			string t = itemsArray.Get(i).GetType();
			t.ToLower();
			if (t == lower)
				count++;
		}

		TraderBuyFix_Log("RECHECK " + itemType + ": " + TraderBuyFix_Describe(newItem) + " countInPlayerTree=" + count);
	}

	string TraderBuyFix_Describe(EntityAI e)
	{
		if (!e)
			return "null";

		int lo, hi;
		e.GetNetworkID(lo, hi);

		string parent = "none";
		EntityAI p = e.GetHierarchyParent();
		if (p)
			parent = p.GetType();

		string root = "none";
		EntityAI r = e.GetHierarchyRoot();
		if (r)
			root = r.GetType();

		InventoryLocation cur = new InventoryLocation();
		string locStr = "?";
		if (e.GetInventory() && e.GetInventory().GetCurrentInventoryLocation(cur))
			locStr = InventoryLocation.DumpToStringNullSafe(cur);

		return "type=" + e.GetType() + " netID=" + lo + ":" + hi + " parent=" + parent + " root=" + root + " pendingDelete=" + e.IsPendingDeletion() + " pos=" + e.GetPosition().ToString() + " loc=" + locStr;
	}

	void TraderBuyFix_Log(string msg)
	{
		Print("[TraderBuyFix] " + msg);
	}
}
