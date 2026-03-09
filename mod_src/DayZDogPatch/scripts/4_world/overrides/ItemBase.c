modded class ItemBase extends InventoryItem
{

    override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
    {
        super.EEItemLocationChanged(oldLoc,newLoc);
        
        
        if ( !GetGame().IsServer() )
        {
            return;
        }
        
        if (oldLoc.GetType() == InventoryLocationType.ATTACHMENT && newLoc.GetType() == InventoryLocationType.ATTACHMENT )
        {
            if (oldLoc.GetParent())
            {
                dog_shed_small doghouse = dog_shed_small.Cast(oldLoc.GetParent());
                PlayerBase player = PlayerBase.Cast(newLoc.GetParent());
                if (player && doghouse)
                {
					if (dog_shed_big.Cast(doghouse))
					{
						player.InitialDogSpawn(doghouse.ModelToWorld("0 0.2 0"));
					}
					else if(!dog_shed_big.Cast(doghouse))
					{
						player.InitialDogSpawn(doghouse.ModelToWorld("2 0 0"));
					}
					//player.InitialDogSpawn(doghouse.ModelToWorld("2 0 0"));
					doghouse.SetDogOwnerSteamID("0");
					DogPreview_Base.Cast(this).SetOwnerSteamID(player.GetIdentity().GetPlainId());
					//player.SetDogAttLock(true);
                }
				doghouse = dog_shed_small.Cast(newLoc.GetParent());
				player = PlayerBase.Cast(oldLoc.GetParent());
				if (player && doghouse)
                {
					//save and delete
					player.StoreDogInventoryTo(this);
					player.StoreDogHealthTo(this);
					DogPreview_Base.Cast(this).SetOwnerSteamID(player.GetIdentity().GetPlainId());
					doghouse.SetDogOwnerSteamID(player.GetIdentity().GetPlainId());
					doghouse.SetDogOwnerID();
					//DoggoLogger.Log("ITEMBASE SET STEAMID " + player.GetIdentity().GetPlainId());
					GetGame().ObjectDelete(player.GetMyDog());
					//doghouse.SetLockAtt(true);

				}
            }
        }
		
    }
}