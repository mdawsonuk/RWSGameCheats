#pragma once

#include "../common.h"

class CGlobalVarsBase;
class ClientClass;
class bf_write;
class bf_read;
class CViewSetup;
class QAngle;
class CEngineSprite;
class CSaveRestoreData;
class CStandardRecvProxies;
class KeyValues;
class IReplayFactory;
class IMaterialProxy;
class IClientEntity;
class CJob;
class IScaleformSlotInitController;
class IConVar;
class CSVCMsg_HltvReplay;
class CEngineGotvSyncPacket;

// TODO: Do we ever need the actual definitions of these?
template<class T, class I = int>
class CUtlMemory;
template<class T, class A = CUtlMemory<T>>
class CUtlVector;
template<class T, int nAlignment = 16>
class CUtlMemoryAligned;

struct ScreenFade_t;
struct vrect_t;
struct datamap_t;
struct typedescription_t;
struct WriteReplayScreenshotParams_t;
struct ClientDLLObserverMode_t;
struct InputEvent_t;
struct SpatializationInfo_t;
struct Frustum_t;
struct PublishedFileId_t;
struct CDemoPlaybackParameters_t;

enum ButtonCode_t : int;
enum ClientFrameStage_t : int;

namespace vgui
{
	typedef uintp VPANEL;
}


//-----------------------------------------------------------------------------
// Purpose: Interface exposed from the client .dll back to the engine
//-----------------------------------------------------------------------------
class IBaseClientDLL
{
public:
	// Connect appsystem components, get global interfaces, don't run any other init code
	virtual int				Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;
	virtual void            Disconnect() = 0;

	// run other init code here
	virtual int				Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;

	virtual void			PostInit() = 0;

	// Called once when the client DLL is being unloaded
	virtual void			Shutdown(void) = 0;

	// Called at the start of each level change
	virtual void			LevelInitPreEntity(char const* pMapName) = 0;
	// Called at the start of a new level, after the entities have been received and created
	virtual void			LevelInitPostEntity() = 0;
	// Called at the end of a level
	virtual void			LevelShutdown(void) = 0;

	// Request a pointer to the list of client datatable classes
	virtual ClientClass* GetAllClasses(void) = 0;

	// Called once per level to re-initialize any hud element drawing stuff
	virtual int				HudVidInit(void) = 0;
	// Called by the engine when gathering user input
	virtual void			HudProcessInput(bool bActive) = 0;
	// Called oncer per frame to allow the hud elements to think
	virtual void			HudUpdate(bool bActive) = 0;
	// Reset the hud elements to their initial states
	virtual void			HudReset(void) = 0;
	// Display a hud text message
	virtual void			HudText(const char* message) = 0;

	// Draw the console overlay?
	virtual bool			ShouldDrawDropdownConsole() = 0;

	// Mouse Input Interfaces
	// Activate the mouse (hides the cursor and locks it to the center of the screen)
	virtual void			IN_ActivateMouse(void) = 0;
	// Deactivates the mouse (shows the cursor and unlocks it)
	virtual void			IN_DeactivateMouse(void) = 0;
	// This is only called during extra sound updates and just accumulates mouse x, y offets and recenters the mouse.
	//  This call is used to try to prevent the mouse from appearing out of the side of a windowed version of the engine if 
	//  rendering or other processing is taking too long
	virtual void			IN_Accumulate(void) = 0;
	// Reset all key and mouse states to their initial, unpressed state
	virtual void			IN_ClearStates(void) = 0;
	// If key is found by name, returns whether it's being held down in isdown, otherwise function returns false
	virtual bool			IN_IsKeyDown(const char* name, bool& isdown) = 0;
	// Raw keyboard signal, if the client .dll returns 1, the engine processes the key as usual, otherwise,
	//  if the client .dll returns 0, the key is swallowed.
	virtual int				IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;

	// This function is called once per tick to create the player CUserCmd (used for prediction/physics simulation of the player)
	// Because the mouse can be sampled at greater than the tick interval, there is a separate input_sample_frametime, which
	//  specifies how much additional mouse / keyboard simulation to perform.
	virtual void			CreateMove(
		int sequence_number,			// sequence_number of this cmd
		float input_sample_frametime,	// Frametime for mouse input sampling
		bool active) = 0;				// True if the player is active (not paused)

	// If the game is running faster than the tick_interval framerate, then we do extra mouse sampling to avoid jittery input
	//  This code path is much like the normal move creation code, except no move is created
	virtual void			ExtraMouseSample(float frametime, bool active) = 0;

	// Encode the delta (changes) between the CUserCmd in slot from vs the one in slot to.  The game code will have
	//  matching logic to read the delta.
	virtual bool			WriteUsercmdDeltaToBuffer(int nSlot, bf_write* buf, int from, int to, bool isnewcommand) = 0;
	// Demos need to be able to encode/decode CUserCmds to memory buffers, so these functions wrap that
	virtual void			EncodeUserCmdToBuffer(int nSlot, bf_write& buf, int slot) = 0;
	virtual void			DecodeUserCmdFromBuffer(int nSlot, bf_read& buf, int slot) = 0;

	// Set up and render one or more views (e.g., rear view window, etc.).  This called into RenderView below
	virtual void			View_Render(vrect_t* rect) = 0;

	// Allow engine to expressly render a view (e.g., during timerefresh)
	// See IVRenderView.h, PushViewFlags_t for nFlags values
	virtual void			RenderView(const CViewSetup& view, int nClearFlags, int whatToDraw) = 0;

	// Apply screen fade directly from engine
	virtual void			View_Fade(ScreenFade_t* pSF) = 0;

	// The engine has parsed a crosshair angle message, this function is called to dispatch the new crosshair angle
	virtual void			SetCrosshairAngle(const QAngle& angle) = 0;

	// Sprite (.spr) model handling code
	// Load a .spr file by name
	virtual void			InitSprite(CEngineSprite* pSprite, const char* loadname) = 0;
	// Shutdown a .spr file
	virtual void			ShutdownSprite(CEngineSprite* pSprite) = 0;
	// Returns sizeof( CEngineSprite ) so the engine can allocate appropriate memory
	virtual int				GetSpriteSize(void) const = 0;

	// Called when a player starts or stops talking.
	// entindex is -1 to represent the local client talking (before the data comes back from the server). 
	// entindex is -2 to represent the local client's voice being acked by the server.
	// entindex is GetPlayer() when the server acknowledges that the local client is talking.
	virtual void			VoiceStatus(int entindex, int iSsSlot, qboolean bTalking) = 0;

	// returns false if the player can't hear the other client due to game rules (eg. the other team)
	virtual bool			PlayerAudible(int iPlayerIndex) = 0;

	// Networked string table definitions have arrived, allow client .dll to 
	//  hook string changes with a callback function ( see INetworkStringTableClient.h )
	virtual void			InstallStringTableCallback(char const* tableName) = 0;

	// Notification that we're moving into another stage during the frame.
	virtual void			FrameStageNotify(ClientFrameStage_t curStage) = 0;

	// The engine has received the specified user message, this code is used to dispatch the message handler
	virtual bool			DispatchUserMessage(int msg_type, int32 nFlags, int size, const void* msg) = 0;	// New protobuf version

	// Save/restore system hooks
	virtual CSaveRestoreData* SaveInit(int size) = 0;
	virtual void			SaveWriteFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			SaveReadFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int) = 0;
	virtual void			PreSave(CSaveRestoreData*) = 0;
	virtual void			Save(CSaveRestoreData*) = 0;
	virtual void			WriteSaveHeaders(CSaveRestoreData*) = 0;
	virtual void			ReadRestoreHeaders(CSaveRestoreData*) = 0;
	virtual void			Restore(CSaveRestoreData*, bool) = 0;
	virtual void			DispatchOnRestore() = 0;

	// Hand over the StandardRecvProxies in the client DLL's module.
	virtual CStandardRecvProxies* GetStandardRecvProxies() = 0;

	// save game screenshot writing
	virtual void			WriteSaveGameScreenshot(const char* pFilename) = 0;

	// Given a list of "S(wavname) S(wavname2)" tokens, look up the localized text and emit
	//  the appropriate close caption if running with closecaption = 1
	virtual void			EmitSentenceCloseCaption(char const* tokenstream) = 0;
	// Emits a regular close caption by token name
	virtual void			EmitCloseCaption(char const* captionname, float duration) = 0;

	// Returns true if the client can start recording a demo now.  If the client returns false,
	// an error message of up to length bytes should be returned in errorMsg.
	virtual bool			CanRecordDemo(char* errorMsg, int length) const = 0;

	// Give the Client a chance to do setup/cleanup.
	virtual void			OnDemoRecordStart(char const* pDemoBaseName) = 0;
	virtual void			OnDemoRecordStop() = 0;
	virtual void			OnDemoPlaybackStart(char const* pDemoBaseName) = 0;
	virtual void			OnDemoPlaybackRestart() = 0;
	virtual void			OnDemoPlaybackStop() = 0;
	virtual void			SetDemoPlaybackHighlightXuid(uint64 xuid, bool bLowlights) = 0;
	virtual void			ShowHighlightSkippingMessage(bool bState, int nCurrentTick = 0, int nTickStart = 0, int nTickStop = 0) = 0;

	// Demo polish callbacks.
	virtual void			RecordDemoPolishUserInput(int nCmdIndex) = 0;

	// Cache replay ragdolls
	virtual bool			CacheReplayRagdolls(const char* pFilename, int nStartTick) = 0;

	// Send a message to the Replay UI
	virtual void			ReplayUI_SendMessage(KeyValues* pMsg) = 0;

	// Get the client replay factory
	virtual IReplayFactory* GetReplayFactory() = 0;

	// Clear out the local player's replay pointer so it doesn't get deleted
	virtual void			ClearLocalPlayerReplayPtr() = 0;

	// Get client screen dimensions
	virtual int				GetScreenWidth() = 0;
	virtual int				GetScreenHeight() = 0;

	// Added interface
	// save game screenshot writing
	virtual void			WriteSaveGameScreenshotOfSize(const char* pFilename, int width, int height, bool bCreatePowerOf2Padded = false, bool bWriteVTF = false) = 0;

	// Write a .VTF screenshot to disk for the replay system
	virtual void			WriteReplayScreenshot(WriteReplayScreenshotParams_t& params) = 0;

	// Reallocate memory for replay screenshots - called if user changes resolution or if the convar "replay_screenshotresolution" changes
	virtual void			UpdateReplayScreenshotCache() = 0;

	// Gets the current view
	virtual bool			GetPlayerView(CViewSetup& playerView) = 0;

	virtual bool			ShouldHideLoadingPlaque(void) = 0;

	virtual void			InvalidateMdlCache() = 0;

	virtual void			IN_SetSampleTime(float frametime) = 0;

	virtual void			OnActiveSplitscreenPlayerChanged(int nNewSlot) = 0;
	// We are entering into/leaving split screen mode (or # of players is changing)
	virtual void			OnSplitScreenStateChanged() = 0;

	virtual int				GetSpectatorTarget(ClientDLLObserverMode_t* pObserverMode) = 0;

	virtual void			CenterStringOff() = 0;

	virtual void			OnScreenSizeChanged(int nOldWidth, int nOldHeight) = 0;

	virtual IMaterialProxy* InstantiateMaterialProxy(const char* proxyName) = 0;

	virtual vgui::VPANEL	GetFullscreenClientDLLVPanel(void) = 0;

	// The engine wants to mark two entities as touching
	virtual void			MarkEntitiesAsTouching(IClientEntity* e1, IClientEntity* e2) = 0;

	virtual void			OnKeyBindingChanged(ButtonCode_t buttonCode, char const* pchKeyName, char const* pchNewBinding) = 0;

	virtual void			SetBlurFade(float scale) = 0;

	virtual void			ResetHudCloseCaption() = 0;

	// Called by the engine to allow the new GameUI to handle key events
	// Function must return true if the key event was handled
	virtual bool			HandleGameUIEvent(const InputEvent_t& event) = 0;

	// check if the client needs to alter non-entity associated sound locations (portal2, multiple sources, etc.)
	virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;

	// sending a hint to the hud
	virtual void			Hud_SaveStarted() = 0;

	virtual void			ShutdownMovies() = 0;

	virtual void			GetStatus(char* buffer, int bufsize) = 0;

	// find out if the chat display is up
	virtual bool IsChatRaised(void) = 0;
	virtual bool IsRadioPanelRaised(void) = 0;
	virtual bool IsBindMenuRaised(void) = 0;
	virtual bool IsTeamMenuRaised(void) = 0;
	virtual bool IsLoadingScreenRaised(void) = 0;

	virtual bool IsBuildWRThreaded(void) = 0;
	virtual void QueueBuildWorldListJob(CJob* pJob) = 0;
	virtual void CacheFrustumData(const Frustum_t& frustum, const CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t, 16 > >& aeraFrustums) = 0;
	virtual const Frustum_t* GetBuildViewFrustum(void) const = 0;
	virtual const CUtlVector< Frustum_t, CUtlMemoryAligned< Frustum_t, 16 > >* GetBuildViewAeraFrustums(void) const = 0;

	virtual bool IsSubscribedMap(const char* pchMapName, bool bOnlyOnDisk) = 0;
	virtual bool IsFeaturedMap(const char* pchMapName, bool bOnlyOnDisk) = 0;

	// Gets file info for a workshop map, downloads latest version if needed
	virtual void DownloadCommunityMapFile(PublishedFileId_t id) = 0;

	// Return download progress from 0.0 - 1.0, or -1.0 on error
	virtual float GetUGCFileDownloadProgress(PublishedFileId_t id) = 0;

	virtual void RecordUIEvent(const char* szEvent) = 0;

	// Inventory access
	virtual float FindInventoryItemWithMaxAttributeValue(char const* szItemType, char const* szAttrClass) = 0;

	// Inventory access
	virtual void DetermineSubscriptionKvToAdvertise(KeyValues* kvLocalPlayer) = 0;

	// Evidence validation
	virtual bool ValidateSignedEvidenceHeader(char const* szKey, void const* pvHeader, CDemoPlaybackParameters_t* pPlaybackParameters) = 0;
	virtual void PrepareSignedEvidenceData(void* pvData, int numBytes, CDemoPlaybackParameters_t const* pPlaybackParameters) = 0;
	virtual bool ShouldSkipEvidencePlayback(CDemoPlaybackParameters_t const* pPlaybackParameters) = 0;

	// Scaleform slot controller
	virtual IScaleformSlotInitController* GetScaleformSlotInitController() = 0;

	virtual bool IsConnectedUserInfoChangeAllowed(IConVar* pCvar) = 0;

	virtual char const* GetRichPresenceStatusString() = 0;

	// Returns true if the client can stop recording a demo now.  If the client returns false,
	// an error message of up to length bytes should be returned in errorMsg.
	virtual bool CanStopRecordDemo(char* errorMsg, int length) const = 0;

	virtual void OnHltvReplay(const CSVCMsg_HltvReplay& msg) = 0;
	virtual void OnHltvReplayTick() = 0;
	virtual int GetHltvReplayDelay() = 0;

	virtual void OnDemoPlaybackTimeJump() = 0;
	virtual void OnCommandDuringPlayback(char const* cmd) = 0;

	virtual void RetireAllPlayerDecals(bool bRenderContextValid) = 0;
	virtual void EngineGotvSyncPacket(const CEngineGotvSyncPacket* pPkt) = 0; // Engine asking GC for a sync packet
	virtual int GetInEyeEntity() const = 0;
	virtual void OnTickPre(int tickcount) = 0;
};