modded class MissionGameplay extends MissionBase
{

	protected ref array<ref HackingModNotificationWidget> HackingNotifications = new array<ref HackingModNotificationWidget>;
	
	void MissionGameplay(){
		GetRPCManager().AddRPC( "HACK", "RPCHackingModSettings", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HACK", "RPCHackingModNotifcation", this, SingeplayerExecutionType.Both );
	}
	
	override void OnMissionStart(){
		super.OnMissionStart();
		Print("[HackingMod][Client] Requesting Settings From Server");
		GetRPCManager().SendRPC("HACK", "RPCHackingModSettings", new Param1< HackingModConfig >( NULL ), true, NULL);
	}
	
	
	void RPCHackingModSettings( CallType type,  ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< HackingModConfig > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		m_HackingModConfig = data.param1;
		Print("[HackingMod][Client] Settings Received");
		//m_HackingModConfig.HackableItems.Debug();
	}
	
	void RPCHackingModNotifcation( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param3< string, string, float > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		string Heading = data.param1;
		string Message = data.param2;
		float MessageTime = data.param3;
		if (GetHackingModConfig().Notification == 1) {
			GetGame().Chat( Message, "colorAction");
		} else if (GetHackingModConfig().Notification == 2) {
			int LastIndex = HackingNotifications.Count() - 1;
			if (HackingNotifications.Count() > 1) {
				HackingNotifications.Get(LastIndex).EarlyProccess();
			}
			int nextIndex = HackingNotifications.Count();
			HackingNotifications.Insert(new HackingModNotificationWidget());
			HackingNotifications.Get(nextIndex).Init(Heading, Message, "HackingMod/GUI/Images/hacking.paa", MessageTime);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.DeleteNotification, MessageTime * 1500, false);
		}
	}
		
	void DeleteNotification(){
		if ( HackingNotifications.Count() > 0 ){
			HackingNotifications.RemoveOrdered(0);
		}
	}
}