#ifndef ISTEAMAPPTICKET_H
#define ISTEAMAPPTICKET_H
#pragma once
class ISteamAppTicket
{
public:
    virtual uint32 GetAppOwnershipTicketData( uint32 nAppID, void *pvBuffer, uint32 cbBufferLength, uint32 *piAppId, uint32 *piSteamId, uint32 *piSignature, uint32 *pcbSignature ) = 0;
};
#define STEAMAPPTICKET_INTERFACE_VERSION "STEAMAPPTICKET_INTERFACE_VERSION001"
#endif // ISTEAMAPPTICKET_H
