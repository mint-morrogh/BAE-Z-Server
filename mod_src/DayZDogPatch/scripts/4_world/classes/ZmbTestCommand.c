/*
class ZmbTestCommandScript : DayZInfectedCommandScript
{
    private bool m_TagActive;
    private InfectedST m_ST;
    private DayZInfectedInputController m_Input;

    void ZmbTestCommandScript(DayZInfected pInfected, InfectedST table)
    {
        m_Input = pInfected.GetInputController();
        m_ST = table;
    }

    override void OnActivate()
    {
        m_Input.OverrideMovementSpeed(true, 0.0);
        m_ST.CallAttack( this, 3, 0);
		DoggoLogger.Log("ZMB COMMAND");
    }

    override void OnDeactivate()
    {
        m_Input.OverrideMovementSpeed(false, 0.0);
    }

    override void PrePhysUpdate(float pDt)
    {
        //PrePhys_IsTag always return false regardless of the presence of the tag!

        m_TagActive = PrePhys_IsTag( m_ST.GetAttackTag() );
    }

    override bool PostPhysUpdate(float pDt)
    {     
        //m_TagActive always false 
        return m_TagActive;
    }
};
*/