//tem fix until windstride clothing will be fixed

modded class PlayerContainer
{
    
	override void ItemAttached(EntityAI item, string slot_name)
    {
        DogPreview_Base dogItem;
        
        if (Class.CastTo(dogItem, item))
        {
            int slot = InventorySlots.GetSlotIdFromString( slot_name );
            SlotsIcon icon = m_InventorySlots.Get( slot );
            
            if( icon )
            {
                if( icon.IsReserved())
                    m_Player.GetOnAttachmentReleaseLock().Invoke(item, slot);
                
                icon.Init( item );

                if( m_ShowedItems.Contains( item ) == false )
                {
                    icon.GetRadialIconPanel().Show( true );
                    RecomputeOpenedContainers();
                    Refresh();
                }
            }
        }
        else
        {
            super.ItemAttached(item, slot_name);
        }
    }
	
	override void MouseClick( Widget w, int x, int y, int button )
	{
		SlotsIcon icon;
		w.GetUserData(icon);
		
		DogPreview_Base dog;
		
		if (icon)
		{
			dog = DogPreview_Base.Cast(icon.GetEntity());
			if (dog && button == MouseState.LEFT)
			{
				ItemManager.GetInstance().SetWidgetDraggable(w, false);
				return;
			}
		}
		super.MouseClick( w, x, y, button ); 
	}
	
	override void DoubleClick(Widget w, int x, int y, int button)
	{
		SlotsIcon icon;
		w.GetUserData(icon);
		
		DogPreview_Base dog;
		
		if (icon)
		{
			dog = DogPreview_Base.Cast(icon.GetEntity());
			if (dog && button == MouseState.LEFT)
			{
				return;
			}
		}
		super.DoubleClick( w, x, y, button ); 	
	}
	
}

//taky ne
modded class AttachmentCategoriesRow
{
	/*
	override void RefreshSlot( int row, int column, int slot_id, string slot_name )
	{
		SlotsIcon icon = m_AttachmentsContainer.GetSlotsIcon(row, column);
		int slot_id_ = icon.GetSlotID();
		EntityAI item = m_Entity.GetInventory().FindAttachment( slot_id_ );
		DogPreview_Base dog;
		if (Class.CastTo(dog, item))
		{
			icon.GetMountedWidget().Show( true );
			icon.GetRender().GetParent().ClearFlags( WidgetFlags.DRAGGABLE );
			return;
		}
		super.RefreshSlot( row, column, slot_id, slot_name );
	}
	*/
	override void RefreshSlot( int row, int column, int slot_id, string slot_name )
	{
		super.RefreshSlot( row, column, slot_id, slot_name );
		
		SlotsIcon icon = m_AttachmentsContainer.GetSlotsIcon(row, column);
		int slot_id_ = icon.GetSlotID();
		EntityAI item = m_Entity.GetInventory().FindAttachment( slot_id_ );
		ContainerWithCargo cont;
		
		if (!m_Entity.CanDisplayAttachmentSlot(slot_id_))
		{
			icon.Clear();
			icon.GetMainWidget().Show( false );
		}
		else if (!item)
		{
			if (icon.GetEntity())
			{
				icon.ClearRemainSelected();
				
				cont = m_AttachmentCargos.Get(slot_id_);
				if (cont)
				{
					m_AttachmentCargos.Remove(slot_id_);
					this.Remove(cont);
					( Container.Cast( cont.m_Parent ) ).Remove( cont );
				}
			}
			icon.GetMainWidget().Show(true);
		}
		else
		{
			icon.GetMainWidget().Show( true );
			if (icon.GetEntity() != item)
			{
				icon.Init( item );
			}
			else
			{
				icon.Refresh();
			}
			
			cont = m_AttachmentCargos.Get(slot_id_);
			if( cont && cont.GetEntity() != item)
			{
				m_AttachmentCargos.Remove(slot_id_);
				( Container.Cast( cont.m_Parent ) ).Remove( cont );
				this.Remove(cont);
				cont = null;
			}
			
			if( !cont )
			{
				if( item.GetInventory().GetCargo() && m_AttachmentCargos )
				{
					cont = new ContainerWithCargo( this, true );
					cont.GetRootWidget().SetSort( m_AttachmentCargos.Count() + 10 );
					cont.SetEntity( item );
					cont.SetSlotIcon( icon );
					
					m_AttachmentCargos.Insert( slot_id_, cont );
					icon.SetContainer(cont);
					
					SetOpenForSlotIcon(cont.IsOpened(),icon);
				}
			}
			
			if (cont)
			{
				icon.GetRadialIconPanel().Show( cont.IsDisplayable() );
			}
				
			string slot_name_ 			= InventorySlots.GetSlotName(slot_id_);
			bool draggable				= true;
			bool can_be_removed			= item.GetInventory().CanRemoveEntity();
			bool in_hands_condition		= m_Entity.GetHierarchyRoot() == GetGame().GetPlayer();
			bool in_vicinity_condition	= AttachmentsOutOfReach.IsAttachmentReachable( m_Entity, slot_name_ );
			if( m_Entity.GetInventory().GetSlotLock( slot_id_ ) && ItemManager.GetInstance().GetDraggedItem() != item )
			{
				icon.GetMountedWidget().Show( true );
				draggable = false;
			}
			else
			{
				icon.GetMountedWidget().Show( false );
			}
			
			if( !m_Entity.CanReleaseAttachment( item ) )
			{
				draggable = false;
			}
			
			if ((in_hands_condition || in_vicinity_condition) && can_be_removed)
			{
				icon.GetOutOfReachWidget().Show( false );
			}
			else
			{
				icon.GetOutOfReachWidget().Show( true );
				draggable = false;
			}
			DogPreview_Base dog = DogPreview_Base.Cast(icon.GetEntity());
			#ifdef DAYZ_HORSE
			HorsePreview_Base horse = HorsePreview_Base.Cast(icon.GetEntity());
			if( draggable && !dog && !horse)
			{
				icon.GetRender().GetParent().SetFlags( WidgetFlags.DRAGGABLE );
			}
			else
			{
				icon.GetRender().GetParent().ClearFlags( WidgetFlags.DRAGGABLE );
			}
			#else
			if( draggable && !dog)
			{
				icon.GetRender().GetParent().SetFlags( WidgetFlags.DRAGGABLE );
			}
			else
			{
				icon.GetRender().GetParent().ClearFlags( WidgetFlags.DRAGGABLE );
			}	
			#endif
		}
	}
	
	override void DoubleClick(Widget w, int x, int y, int button)
	{
		SlotsIcon icon;
		w.GetUserData(icon);
		
		DogPreview_Base dog;
		
		if (icon)
		{
			dog = DogPreview_Base.Cast(icon.GetEntity());
			if (dog && button == MouseState.LEFT)
			{
				return;
			}
		}
		super.DoubleClick( w, x, y, button ); 	
	}
	
}