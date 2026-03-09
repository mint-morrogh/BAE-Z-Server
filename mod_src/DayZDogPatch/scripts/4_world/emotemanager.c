modded class EmoteManager
{
	override bool PlayEmote( int id )
	{
		if (id == EmoteConstants.ID_EMOTE_DANCE)
		{
			m_Player.SetDance(true);
		}
		return super.PlayEmote(id);
	}
	
	override void OnEmoteEnd()
	{
		if ( m_Player.HaveDog() && GetGame().IsServer() && m_Player.GetMyDog() && m_Player.GetMyDog().CanListenEmotes() )
		{
			if (m_CurrentGestureID == EmoteConstants.ID_EMOTE_COME)
			{
				m_Player.HandleFollow();
			}					
			else if (m_CurrentGestureID == EmoteConstants.ID_EMOTE_DOWN)
			{
				m_Player.HandleLayDown();
			}
			else if (m_CurrentGestureID == EmoteConstants.ID_EMOTE_MOVE)
			{
				m_Player.HandlePatrol();
			}
			else if (m_CurrentGestureID == EmoteConstants.ID_EMOTE_DANCE)
			{
				m_Player.SetDance(false);
			}			
			
		}
		
		
		super.OnEmoteEnd();
	}
	override void Update( float deltaT )
	{
		if (m_CurrentGestureID == EmoteConstants.ID_EMOTE_POINT && GetGame().IsClient())
		{
			//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Player pointing", ""));
			m_Player.SetPointing(true);
		} else { m_Player.SetPointing(false); }
		super.Update(deltaT);
	}

}
