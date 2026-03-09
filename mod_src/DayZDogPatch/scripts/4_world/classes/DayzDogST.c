class DayzDogST
{
	private int m_CMD_Jump;
	private int m_CMD_Attack;
	private int m_CMD_AttackSuccess;
	
	protected float m_VAR_Speed;
	protected float m_VAR_Turn;
	protected float m_VAR_LookXAngle;
	protected int m_VAR_Behavior;
	protected int m_VAR_AlertLevel;
	protected int m_VAR_behaviorSlotID;
	protected int m_TAG_Attacking;
	protected bool m_VAR_ShouldSwim;

	void DayzDogST( Dayz_Doggo dog )
	{
		DayZCreatureAnimInterface ai = dog.GetAnimInterface();
		
		m_CMD_Jump = ai.BindCommand( "CMD_JumpType" );
		m_CMD_Attack = ai.BindCommand( "CMD_Attack" );
		m_CMD_AttackSuccess = ai.BindCommand( "CMD_AttackSuccess" );
		
		m_VAR_Speed = ai.BindVariableFloat( "speed" );
		m_VAR_Turn = ai.BindVariableFloat( "turn" );
		m_VAR_Behavior = ai.BindVariableInt( "behaviorAction" );
		m_VAR_AlertLevel = ai.BindVariableInt( "alertLevel" );
		m_VAR_behaviorSlotID = ai.BindVariableInt( "behaviorSlotID" );
		m_VAR_LookXAngle = ai.BindVariableFloat( "lookXAngle" );
		
		m_TAG_Attacking = ai.BindTag( "TagAttacking" );
		m_VAR_ShouldSwim = ai.BindVariableBool( "shouldSwim" );
	}
	
	void CallJump( DayZAnimalCommandScript script, int param1, float param2 )
	{
		script.PreAnim_CallCommand( m_CMD_Jump, param1, param2 );
	}
	
	void CallAttack( DayZAnimalCommandScript script, int param1, float param2 )
	{
		script.PreAnim_CallCommand( m_CMD_Attack, param1, param2 );
	}
	
	void CallAttackVanilla( DayZAnimalCommandAttackModdable script, int param1, float param2 )
	{
		script.PreAnim_CallCommand( m_CMD_Attack, param1, param2 );
	}
	
	void CallAttackSuccess( DayZAnimalCommandScript script, int param1, float param2 )
	{
		script.PreAnim_CallCommand( m_CMD_AttackSuccess, param1, param2 );
	}
	
	void SetSpeed( DayZAnimalCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_Speed, param );
	}
	
	void SetSpeedMove( DayZAnimalCommandMoveModdable script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_Speed, param );
	}
	
	void SetTurn( DayZAnimalCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_Turn, param );
	}
	
	void SetBehavior( DayZAnimalCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_Behavior, param );
	}
	
	void SetAlertLevelM( DayZAnimalCommandMoveModdable script, int param )
	{
		script.PreAnim_SetInt( m_VAR_AlertLevel, param );
	}
	
	void SetAlertLevel( DayZAnimalCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_AlertLevel, param );
	}
	
	void SetBehaviorSlotIDM( DayZAnimalCommandMoveModdable script, int param )
	{
		script.PreAnim_SetInt( m_VAR_behaviorSlotID, param );
	}
	
	void SetLookXAngle( DayZAnimalCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_LookXAngle, param );
	}

	int GetAttackTag()
    {
        return m_TAG_Attacking;
    }
	
	void SetSwiming( DayZAnimalCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_ShouldSwim, param );
	}
	
	void SetSwimingM( DayZAnimalCommandMoveModdable script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_ShouldSwim, param );
	}

};
/*
class InfectedST
{
    protected int m_CMD_Attack;
    protected int m_TAG_Attack;

    void InfectedST( ZombieBase zmb )
    {
        DayZCreatureAnimInterface anim_i = zmb.GetAnimInterface();
        m_CMD_Attack = anim_i.BindCommand( "CMD_ATTACK" );
        m_TAG_Attack = anim_i.BindTag("TagAttack");
    }

    void CallAttack( DayZInfectedCommandScript script, int pParamInt, float pParamFloat)
    {
        script.PreAnim_CallCommand( m_CMD_Attack, pParamInt, pParamFloat );
    }   

    int GetAttackTag()
    {
        return m_TAG_Attack;
    }
	
};
*/