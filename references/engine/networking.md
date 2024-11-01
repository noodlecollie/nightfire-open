## Engine - Sending Entity Data to Clients

``` c
// Main server frame.
Host_ServerFrame();
 // Send messages to clients who were involved in the frame.
 SV_SendClientMessages();
  // For each client, send a datagram.
  SV_SendClientDatagram();
   // Write data specific to the client.
   SV_WriteClientdataToMessage();
    // Call into the game DLL to get game-specific updates.
    svgame.dllFuncs.pfnUpdateClientData();
    // Write delta-compressed version of local clientdata_t.
    MSG_WriteClientData();
    // If clients are predicting weapons, get weapon data from the game DLL.
    svgame.dllFuncs.pfnGetWeaponData();
   // Write data for entities that the client should be informed about.
   SV_WriteEntitiesToClient();
    // Add all directly visible entities.
    SV_AddEntitiesToPacket();
     // Gets the PVS/PAS to use from the game, which can override them if it wants.
     svgame.dllFuncs.pfnSetupVisbility();
     // Ask the game DLL to add the actual entity data.
     svgame.dllFuncs.pfnAddToFullPack();
    // Write them into the message.
    SV_EmitPacketEntities();
    // Write events into the message.
    SV_EmitEvents();
    // Write information about connection quality to other clients, if required.
    SV_EmitPings();
```
