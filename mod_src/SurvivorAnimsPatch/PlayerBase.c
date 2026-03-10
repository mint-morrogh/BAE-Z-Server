modded class PlayerBase
{
	protected int m_SAEmoteID;
	protected int m_FacialState;
	
	override void Init()
	{
		super.Init();
		m_FacialState = -1;
	}
	
	override void SetActions()
	{
		super.SetActions();
	}
	int GetSAEmoteID()
	{
		return m_SAEmoteID;
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		// BAE-Z patch: skip the ENTIRE CommandHandler chain for non-local
		// player entities on client (AI proxies and remote players).
		//
		// On a multiplayer client the engine calls CommandHandler on ALL
		// player entities (including Expansion AI proxies) every frame.
		// The full vanilla chain (DayZPlayerImplement -> PlayerBase) calls
		// HandleInventory, HandleWeapons, InjuryHandler.Update,
		// ShockHandler.Update, etc. — all of which internally touch
		// Object::GetHealth01 on the entity. AI proxy entities do NOT have
		// health data synced to the client, so each GetHealth01 call
		// triggers the native engine error "Object::GetHealth01 cannot be
		// called on client." With dozens of AI entities, this produces 76+
		// errors per session that corrupt the engine's internal memory,
		// leading to access violations in unrelated systems like
		// WeaponManager::GetCurrentModeName.
		//
		// For remote players / AI proxies, the engine replicates animations
		// and movement via the network — CommandHandler is not needed for
		// visual correctness. The eAIBase.CommandHandler on the server side
		// continues to work because this guard only fires on client.
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
		{
			if (GetGame().GetPlayer() != this)
				return;
		}

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}
	
	int GetFacialState()
	{
		return m_FacialState;
	}
	
	void SetFacialState(int state)
	{
		m_FacialState = state;
	}
	
	override bool ModCommandHandlerAfter (float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		// BAE-Z patch: block ALL ModCommandHandlerAfter processing on
		// non-local entities on client. Expansion AI proxies trigger
		// engine-level "GetHealth01 cannot be called on client" errors
		// that accumulate and destabilize the engine, eventually causing
		// access violation crashes (e.g. WeaponManager::GetCurrentModeName).
		// Guard MUST be before super to prevent the entire chain.
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
		{
			if (GetGame().GetPlayer() != this)
				return false;
		}

		//! MUST BE HERE TO NOT DISABLE OTHER MODS
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{
			HumanAnimInterface hai = GetAnimInterface();
			if (hai)
			{
				TAnimGraphVariable m_VarFacialState = hai.BindVariableInt("FacialState");
				AnimSetInt(m_VarFacialState, GetFacialState());
			}
			return true;
		}

		return false;	// not handled
	}
	
	override void OnParticleEvent(string pEventType, string pUserString, int pUserInt)
	{
		// BAE-Z patch: guard against bone/health access on AI proxies
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
		{
			if (GetGame().GetPlayer() != this)
			{
				super.OnParticleEvent(pEventType, pUserString, pUserInt);
				return;
			}
		}

		super.OnParticleEvent(pEventType ,pUserString, pUserInt);

		if (!GetGame().IsDedicatedServer())
		{
			if (pUserInt == 85501)
			{
				PlayerBase player = PlayerBase.Cast(this);
				int boneIdx = player.GetBoneIndexByName("RightWristExtra");

				if (boneIdx != -1)
				{
					EffectParticle eff;

					eff = new EffMoneyRain();

					vector wp = player.GetBonePositionWS(boneIdx);

					eff.SetDecalOwner(player);
					eff.SetAutodestroy(true);
					SEffectManager.PlayInWorld(eff, wp);
				}

			}

		}
	}
	
}