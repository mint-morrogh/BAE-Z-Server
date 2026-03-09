modded class Hologram
{
	
	override string ProjectionBasedOnParent()
	{
		ItemBase item = ItemBase.Cast(m_Player.GetHumanInventory().GetEntityInHands());
		
        dog_shed_big_kit item_in_hands = dog_shed_big_kit.Cast( item );
		dog_shed_wooden_kit wooden_kit_in_hands = dog_shed_wooden_kit.Cast( item );
		dog_shed_small_kit small_kit_in_hands = dog_shed_small_kit.Cast( item );
        if ( item_in_hands )
            return "dog_shed_big";
		if ( wooden_kit_in_hands )
            return "dog_shed_wooden";
		if ( small_kit_in_hands )
            return "dog_shed_small";
        
		return super.ProjectionBasedOnParent();
    }

    override void EvaluateCollision(ItemBase action_item = null)
    {        
		ItemBase item_in_hands = ItemBase.Cast(m_Player.GetHumanInventory().GetEntityInHands());
		
		if (!item_in_hands) 
		{
    		super.EvaluateCollision();
    		return;
		}
		
        if (item_in_hands.IsInherited(dog_shed_big))
        {
            SetIsColliding(false);
            return;
        }    
		if (item_in_hands.IsInherited(dog_shed_wooden))
        {
            SetIsColliding(false);
            return;
        }
		if (item_in_hands.IsInherited(dog_shed_small))
        {
            SetIsColliding(false);
            return;
        }
		if (item_in_hands.IsInherited(DogPainting_Base))
		{
			SetIsColliding(false);
			return;
		}
		
        super.EvaluateCollision();
    }
};