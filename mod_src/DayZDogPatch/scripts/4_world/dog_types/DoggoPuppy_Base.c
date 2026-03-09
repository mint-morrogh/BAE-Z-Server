class DoggoPuppy_Base extends AnimalBase
{
	private ref PuppyST m_AnimST;
	protected AIAgent m_AIAgent;
	private PuppyCommandStay m_CommandStay;

	void DoggoPuppy_Base()
	{
		if (GetGame().IsServer())
		{
			if ( !m_AnimST )
			{
				m_AnimST = new PuppyST( this );
			}
		}
	}
	
	PuppyST GetAnimST()
	{
		return m_AnimST;
	}
	
	PuppyCommandStay StartCommand_PuppyStay()
	{
		if ( !m_AnimST )
			m_AnimST = new PuppyST( this );

		m_CommandStay = PuppyCommandStay.Cast( StartCommand_Script( new PuppyCommandStay( this ) ) );
		
		return m_CommandStay;
	}
	
	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}
		DayZAnimalInputController inputController = GetInputController();
		if (inputController.IsDead())
		{
			return true;
		}
		m_AIAgent = this.GetAIAgent();
		SetLifetime(360000);

		if ( pCurrentCommandID == DayZAnimalConstants.COMMANDID_SCRIPT )
		{
			DayZAnimalCommandScript acs = GetCommand_Script();
			if ( PuppyCommandStay.Cast(acs) != null )
			{
				return true;
			}
		}
		if (m_AIAgent)
		{
			m_AIAgent.SetKeepInIdle(true);
			StartCommand_PuppyStay();
		}
		//inputController.OverrideMovementSpeed(true, 0);
		return false;
	}
};

class Puppy_Shepherd : DoggoPuppy_Base {}