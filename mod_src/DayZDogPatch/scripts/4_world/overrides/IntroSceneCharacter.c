modded class IntroSceneCharacter
{
	protected Object m_DogObj;

	override protected void CharacterLoad( int character_id, vector char_pos, vector char_rot )
	{
		// BAE-Z patch: reimplement vanilla CharacterLoad with null guards
		// instead of calling super, which crashes when m_CharacterDta is
		// null during heavy mod loading (Access violation at 0x8).
		if ( character_id == -1 )
			return;

		CharacterUnload();
		SetCharacterID( character_id );

		if ( m_CharacterDta )
		{
			m_CharacterObj = PlayerBase.Cast( m_CharacterDta.CreateCharacterPerson( character_id ) );
		}

		if ( m_CharacterObj )
		{
			m_CharacterObj.PlaceOnSurface();
			m_CharacterObj.SetPosition(char_pos);
			m_CharacterObj.SetOrientation(char_rot);

			if (m_CharacterObj.HaveDog() && !m_DogObj )
			{
				m_DogObj = g_Game.CreateObjectEx("Doggo_Stay" + m_CharacterObj.GetDogIndex(), m_CharacterObj.ModelToWorld("-0.6 0 -0.5"), ECE_PLACE_ON_SURFACE);
				if (m_DogObj)
					m_DogObj.SetOrientation(char_rot);
			}
		}
	}
}
