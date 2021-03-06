;===============================================================================================
; CDDA.EXE
; Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
;
; Use FMOD stream API To do digital CD playback
;=============================================================================================== 
; PureBasic Port by KarLKoX 
; mail to KarLKoX@ifrance.com for bugs/suggestions

XIncludeFile "../../api/PureBasic/con_struc.pbi"
XIncludeFile "../../api/PureBasic/fmod_proc.pbi"

Global last_openstate.l, this_openstate.l
Global firsttime.b

  OpenConsole()
  ClearConsole()
  ConsoleTitle("PureBasic - CDDA Example")
   
  ; INSTANCIATE FMOD DLL
  LoadFMOD()
  
  result.f = FSOUND_GetVersion()
  ; call fpu register and store (pop) the value
  !FSTP dword [esp]   
  !POP [v_result]     
  If (result < FMOD_VERSION)
    PrintN("Error : You are using the wrong DLL version!  You should be using FMOD " + StrF(FMOD_VERSION))
    Delay(2000)
    CloseConsole()
    End
  EndIf
  
  ; INITIALIZE    
  If FSOUND_Init(44100, 64, 0) <= 0
    PrintN("FSOUND_Init() Error!")
    PrintN(Str(FMOD_ErrorString(FSOUND_GetError())))
    Delay(1000)
    Gosub Exit
  EndIf  
    

  ; LOAD SONG    
  stream = FSOUND_Stream_Open(@"i:*?", #FSOUND_NORMAL | #FSOUND_NONBLOCKING, 0, 0)
  If (stream <= 0)
    PrintN("FSOUND_Stream_Open() Error!")
    PrintN(FMOD_ErrorString(FSOUND_GetError()))
    Delay(1000)
    Gosub Exit
  EndIf
    
  FSOUND_Stream_SetSubStream(stream, 0)
    
  PrintN("=========================================================================")
  PrintN("Press f        Skip forward 2 seconds")
  PrintN("      b        Skip back 2 seconds")
  PrintN("      n        Next track")
  PrintN("      SPACE    Pause/Unpause")
  PrintN("      ESC      Quit")
  PrintN("=========================================================================")

  channel = -1  
Repeat
  
  If (stream And (channel < 0))
    this_openstate = FSOUND_Stream_GetOpenState(stream)
    last_openstate = -1
     
      If (this_openstate = -3)
        If (FSOUND_Stream_FindTagField(stream, 0, "CD_ERROR", @cd_error, 0))
          PrintN("FSOUND_Stream_FindTagField error : " + PeekS(@cd_error))
          Delay(2000)
        Else
          PrintN("ERROR: Couldn't open CDDA stream")
          Delay(2000)
        EndIf

      Gosub Exit
      EndIf
    
      If ((last_openstate <> 0) And (this_openstate = 0))
        firsttime = #True
      
        If (firsttime = #True)
          If (FSOUND_Stream_GetTagField(stream, 0, 0, 0, @cd_device_info, 0) = 0)
            PrintN("FSOUND_Stream_GetTagField error: Couldn't get CD_DEVICE_INFO tag")
            Delay(2000)
            Gosub Exit
          EndIf
        
          ConsoleLocate(0, 7)
          ; Strip dummy characteres
          chaine$ = PeekS(cd_device_info)
          crlf = FindString(chaine$, Chr(13), 0)
          PrintN(Mid(chaine$, 0, crlf - 1))
          PrintN(Mid(chaine$, crlf + 2, Len(chaine$) - crlf))
          PrintN("=========================================================================")
          firsttime = #False
        
          If (FSOUND_Stream_FindTagField(stream, 0, "CD_ERROR", @cd_error, 0) > 0)
            PrintN("FSOUND_Stream_FindTagField error : " + PeekS(cd_error))
            Delay(2000)
            Gosub Exit
          EndIf
        
        EndIf
        
          channel = FSOUND_Stream_PlayEx(#FSOUND_FREE, stream, 0, #True)
          FSOUND_SetPaused(channel, #False)
        
        EndIf
        
    last_openstate = this_openstate
      
  EndIf
    
  keys.s = Left(Inkey(),1)  
  
  If (channel <> -1)
    If keys = " "
      FSOUND_SetPaused(channel, FSOUND_GetPaused(channel) ! 1)
    ElseIf keys = "f"
      FSOUND_Stream_SetTime(stream, FSOUND_Stream_GetTime(stream) + 2000)
    ElseIf keys = "b"
      FSOUND_Stream_SetTime(stream, FSOUND_Stream_GetTime(stream) - 2000)
    ElseIf keys = "n"
      track + 1
      If (track >= FSOUND_Stream_GetNumSubStreams(stream))
        track = 0
      EndIf
      FSOUND_Stream_SetSubStream(stream, track)
      channel = -1      
    EndIf
  EndIf
  
  If (FSOUND_Stream_GetOpenState(stream) = 0)
    ConsoleLocate(0,10)
    PrintN("Track " + Str(track + 1) + "/" + Str(FSOUND_Stream_GetNumSubStreams(stream)) + "  " + RSet$(Str(FSOUND_Stream_GetTime(stream) / 1000 / 60), 2, "0") + ":" + RSet$(Str((FSOUND_Stream_GetTime(stream) / 1000)%60), 2, "0") + "/" + RSet$(Str(FSOUND_Stream_GetLengthMs(stream) / 1000 / 60), 2, "0") + ":" + RSet$(Str((FSOUND_Stream_GetLengthMs(stream) / 1000)%60), 2, "0") + "  " + "cpu " + StrF(FSOUND_GetCPUUsage()))
  EndIf
   
  FSOUND_Update()   
  Delay(10)
  
Until Asc(keys) = 27

Gosub Exit
    
    
Exit:
    ; CLEANUP And SHUTDOWN
If (stream) 
  FSOUND_Stream_Close(stream)
EndIf
FSOUND_Close()
CloseFMOD()
CloseConsole()
End
Return     
; jaPBe Version=1.4.4.25
; Build=51
; FirstLine=0
; CursorPosition=16
; ExecutableFormat=Console
; Executable=E:\Gravure\Prog\Vb\SoundZ\Fmod\fmodapi372win32\fmodapi372win\samplesPureBasic\cdda\cdda.exe
; DontSaveDeclare
; EOF