void main()
{
    // Your existing code...

    // Register your server events with CF
    GetDayZGame().Event_OnRPC.Insert(OnEvent);
}

void OnEvent(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) 
{
    // instantiate your server events class
    ServerEvents serverEvents = new ServerEvents();

    // register the server events with CF
    CF.Event_OnRPC.Insert(serverEvents.OnEvent);
}