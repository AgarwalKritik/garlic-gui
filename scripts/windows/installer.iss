[Setup]
AppName=GarlicGUI
AppVersion={#AppVersion}
WizardStyle=modern
DefaultDirName={autopf}\GarlicGUI
DefaultGroupName=GarlicGUI
UninstallDisplayIcon={app}\GarlicGUI.exe
Compression=lzma2
SolidCompression=yes
OutputBaseFilename=Garlic-GUI-Windows-installer-{#AppVersion}
OutputDir=Output

[Files]
Source: "..\..\build\install_dir\bin\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\GarlicGUI"; Filename: "{app}\GarlicGUI.exe"
Name: "{commondesktop}\GarlicGUI"; Filename: "{app}\GarlicGUI.exe"

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; Check: NeedsAddPath(ExpandConstant('{app}'))

[Code]
function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path', OrigPath)
  then begin
    Result := True;
    exit;
  end;
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;
