// class Set_Dog_Owner
// {	
//     void Set_Dog_Owner()
//     {
// 		GetRPCManager().AddRPC( "dayz_dog", "Set_My_Dog", this, true);
//     }

//     void Set_My_Dog(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
// 	{	
// 		Param1< PlayerBase > data;
//         if ( !ctx.Read( data ) ) return;
		
// 		//PlayerBase Player_Me = data.param1;

// 		data.param1.setMyDog(target);
// 	}
// };

// modded class MissionGameplay
// {
//     ref Set_Dog_Owner m_Set_Dog_Owner;

//     void MissionGameplay()
//     {
//         m_Set_Dog_Owner = new ref Set_Dog_Owner();
//     }
// };

// modded class MissionServer
// {
//     ref Set_Dog_Owner m_Set_Dog_Owner;

//     void MissionServer()
//     {
//         m_Set_Dog_Owner = new ref Set_Dog_Owner();
//     }
// };