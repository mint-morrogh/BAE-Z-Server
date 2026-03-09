class PuppyST
{
	private int m_CMD_Jump;
	private int m_CMD_Attack;
	private int m_CMD_AttackSuccess;
	
	protected float m_VAR_Speed;
	protected float m_VAR_Turn;
	protected float m_VAR_LookXAngle;
	protected float m_VAR_Direction;
	protected int m_VAR_Behavior;
	protected int m_VAR_AlertLevel;
	protected int m_TAG_Attacking;
	protected bool m_VAR_ShouldSwim;

	void PuppyST( AnimalBase animal )
	{
		DayZCreatureAnimInterface ai = animal.GetAnimInterface();
		
		m_CMD_Jump = ai.BindCommand( "CMD_JumpType" );
		m_CMD_Attack = ai.BindCommand( "CMD_Attack" );
		m_CMD_AttackSuccess = ai.BindCommand( "CMD_AttackSuccess" );
		
		m_VAR_Speed = ai.BindVariableFloat( "speed" );
		m_VAR_Turn = ai.BindVariableFloat( "turn" );
		m_VAR_Behavior = ai.BindVariableInt( "behaviorAction" );
		m_VAR_AlertLevel = ai.BindVariableInt( "alertLevel" );
		m_VAR_LookXAngle = ai.BindVariableFloat( "lookXAngle" );
		m_VAR_Direction = ai.BindVariableFloat( "direction" );
		m_VAR_ShouldSwim = ai.BindVariableBool( "shouldSwim" );
		
		m_TAG_Attacking = ai.BindTag( "TagAttacking" );
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
	
	void SetAlertLevel( DayZAnimalCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_AlertLevel, param );
	}
	
	void SetLookXAngle( DayZAnimalCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_LookXAngle, param );
	}

	int GetAttackTag()
    {
        return m_TAG_Attacking;
    }
	
	void SetDirectionM( DayZAnimalCommandMoveModdable script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_Direction, param );
	}
	
	void SetSwimM( DayZAnimalCommandMoveModdable script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_ShouldSwim, param );
	}
	
	void SetSwim( DayZAnimalCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_ShouldSwim, param );
	}
	
	void SetSwimL( DayZAnimalCommandLookAtModdable script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_ShouldSwim, param );
	}

};
