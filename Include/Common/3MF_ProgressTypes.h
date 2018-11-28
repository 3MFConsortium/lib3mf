/*++

Copyright (C) 2018 3MF Consortium

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

Abstract: Progress Types

--*/

#pragma once


namespace NMR
{
	enum ProgressIdentifier {
		PROGRESS_QUERYCANCELED = 0,
		PROGRESS_DONE,
		PROGRESS_CLEANUP,
		PROGRESS_READSTREAM,
		PROGRESS_EXTRACTOPCPACKAGE,
		PROGRESS_READNONROOTMODELS,
		PROGRESS_READROOTMODEL,
		PROGRESS_READRESOURCES,
		PROGRESS_READMESH,
		PROGRESS_READSLICES,
		PROGRESS_READBUILD,
		PROGRESS_CREATEOPCPACKAGE,
		PROGRESS_WRITEMODELSTOSTREAM,
		PROGRESS_WRITEROOTMODEL,
		PROGRESS_WRITENONROOTMODELS,
		PROGRESS_WRITEATTACHMENTS,
		PROGRESS_WRITECONTENTTYPES,
		PROGRESS_WRITENOBJECTS,
		PROGRESS_WRITENODES,
		PROGRESS_WRITETRIANGLES,
		PROGRESS_WRITESLICES
	};
	
	// !!! Matches dll interface type, always modify both
	// If the first parameter is -1, it does not indicate progress;In that case
	// it must only be used to stop the execution of the calling function.
	typedef bool(*Lib3MFProgressCallback)(int, ProgressIdentifier, void*);
};
