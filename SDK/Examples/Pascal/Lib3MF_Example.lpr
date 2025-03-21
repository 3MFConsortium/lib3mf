(*++

Copyright (C) 2019 3MF Consortium (Original Author)

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This file has been generated by the Automatic Component Toolkit (ACT) version 1.6.0-develop.

Abstract: This is an autogenerated Pascal application that demonstrates the
 usage of the Pascal bindings of the 3MF Library

Interface version: 2.4.1

*)

program Lib3MFPascalTest;

uses
	{$IFDEF UNIX}{$IFDEF UseCThreads}
	cthreads,
	{$ENDIF}{$ENDIF}
	Classes, SysUtils, CustApp,
	Unit_Lib3MF
	{ you can add units after this };

type

TLib3MF_Example = class(TCustomApplication)
protected
	procedure DoRun; override;
	procedure TestLib3MF ();
public
	constructor Create(TheOwner: TComponent); override;
	destructor Destroy; override;
end;


procedure TLib3MF_Example.TestLib3MF ();
var
	ALib3MFWrapper: TLib3MFWrapper;
	AMajor, AMinor, AMicro: Cardinal;
	APreReleaseInfo, ABuildInfo: string;
	AVersionString: string;
	ALibPath: string;
begin
	writeln ('loading DLL');
	ALibPath := ''; // TODO add the location of the shared library binary here
	ALib3MFWrapper := TLib3MFWrapper.Create (ALibPath + '/' + 'lib3mf.'); // TODO add the extension of the shared library file here
	try
		writeln ('loading DLL Done');
		ALib3MFWrapper.GetLibraryVersion(AMajor, AMinor, AMicro);
		AVersionString := Format('Lib3MF.version = %d.%d.%d', [AMajor, AMinor, AMicro]);
		if (ALib3MFWrapper.GetPrereleaseInformation(APreReleaseInfo)) then
			AVersionString := AVersionString + '-' + APreReleaseInfo;
		if (ALib3MFWrapper.GetBuildInformation(ABuildInfo)) then
			AVersionString := AVersionString + '-' + ABuildInfo;
		writeln(AVersionString);
	finally
		FreeAndNil(ALib3MFWrapper);
	end;
end;

procedure TLib3MF_Example.DoRun;
begin
	try
		TestLib3MF();
	except
		On E: Exception do
			writeln ('Fatal error: ', E.Message);
	end;
	Terminate
end;

constructor TLib3MF_Example.Create(TheOwner: TComponent);
begin
	inherited Create(TheOwner);
	StopOnException:=True;
end;

destructor TLib3MF_Example.Destroy;
begin
	inherited Destroy;
end;


var
	Application: TLib3MF_Example;
begin
	Application:=TLib3MF_Example.Create(nil);
	Application.Run;
	Application.Free;
end.
