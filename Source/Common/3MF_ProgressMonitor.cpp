/*++

Copyright (C) 2019 3MF Consortium

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

Abstract: Progress Monitor

--*/

#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"

#include <cmath>
#include <algorithm>
#include <limits>

NMR::CProgressMonitor::CProgressMonitor()
{
	m_progressCallback = nullptr;
	m_userData = nullptr;
	m_lastCallbackResult = true;
	m_dProgress = 0;
	m_dProgressMax = 1;
	m_eProgressIdentifier = ProgressIdentifier::PROGRESS_QUERYCANCELED;
}

bool NMR::CProgressMonitor::QueryCancelled(bool throwIfCancelled)
{
	if (m_progressCallback)
	{
		std::unique_lock<std::mutex> lock(m_callbackMutex, std::try_to_lock);
		if (lock) // If another progress callback is happening right _now_, just drop this one
		{
			int nProgress = (int)(100 * m_dProgress / m_dProgressMax);
			m_lastCallbackResult = m_progressCallback(nProgress, ProgressIdentifier::PROGRESS_QUERYCANCELED, m_userData);

			if (throwIfCancelled && m_lastCallbackResult)
				throw CNMRException(NMR_USERABORTED);

			return m_lastCallbackResult;
		}
	}
	return false;
}

bool NMR::CProgressMonitor::ReportProgressAndQueryCancelled(bool throwIfCancelled)
{
	if (m_progressCallback)
	{
		std::unique_lock<std::mutex> lock(m_callbackMutex, std::try_to_lock);
		if (lock) // If another progress callback is happening right _now_, just drop this one
		{
			int nProgress = (int)(100 * m_dProgress / m_dProgressMax);
			m_lastCallbackResult = m_progressCallback(nProgress, m_eProgressIdentifier, m_userData);

			if (throwIfCancelled && m_lastCallbackResult)
				throw CNMRException(NMR_USERABORTED);

			return m_lastCallbackResult;
		}
	}
	return false;
}

bool NMR::CProgressMonitor::WasAborted()
{
	return (m_lastCallbackResult == false);
}

void NMR::CProgressMonitor::SetProgressIdentifier(ProgressIdentifier identifier)
{
	m_eProgressIdentifier = identifier;
}

void NMR::CProgressMonitor::SetMaxProgress(double dProgressMax)
{
	m_dProgressMax = dProgressMax;
}

void NMR::CProgressMonitor::DecreaseMaxProgress(double dMaxProgressDecrement)
{
	m_dProgressMax = std::max(m_dProgressMax - dMaxProgressDecrement, 1.0);
	m_dProgress = std::min(m_dProgressMax, m_dProgress);
}

void NMR::CProgressMonitor::IncrementProgress(double dProgressIncrement)
{
	if (m_progressCallback)
	{
		{
			std::unique_lock<std::mutex> lock(m_callbackMutex, std::try_to_lock);
			m_dProgress = std::min(m_dProgressMax, m_dProgress + dProgressIncrement);
		}
	}
}

void NMR::CProgressMonitor::SetProgressCallback(Lib3MFProgressCallback callback, void * userData)
{
	m_progressCallback = callback;
	m_userData = userData;
	m_lastCallbackResult = true;
}

void NMR::CProgressMonitor::ClearProgressCallback()
{
	SetProgressCallback(nullptr, nullptr);
}

void NMR::CProgressMonitor::GetProgressMessage(NMR::ProgressIdentifier progressIdentifier, std::string& progressString) {
	switch (progressIdentifier) {
		case PROGRESS_QUERYCANCELED: progressString = ""; break;
		case PROGRESS_DONE: progressString = "Done"; break;
		case PROGRESS_CLEANUP: progressString = "Cleaning up"; break;
		case PROGRESS_READSTREAM: progressString = "Reading stream"; break;
		case PROGRESS_EXTRACTOPCPACKAGE: progressString = "Extracting OPC package"; break;
		case PROGRESS_READNONROOTMODELS: progressString = "Reading non-root models"; break;
		case PROGRESS_READROOTMODEL: progressString = "Reading root model"; break;
		case PROGRESS_READRESOURCES: progressString = "Reading resources"; break;
		case PROGRESS_READMESH: progressString = "Reading mesh data"; break;
		case PROGRESS_READSLICES: progressString = "Reading slice data"; break;
		case PROGRESS_READBUILD: progressString = "Reading build definition"; break;
		case PROGRESS_READCUSTOMATTACHMENTS: progressString = "Reading custom attachments"; break;
		case PROGRESS_READTEXTURETACHMENTS: progressString = "Reading texture attachments"; break;
		case PROGRESS_CREATEOPCPACKAGE: progressString = "Creating OPC package"; break;
		case PROGRESS_WRITEMODELSTOSTREAM: progressString = "Writing models to stream"; break;
		case PROGRESS_WRITEROOTMODEL: progressString = "Writing root model"; break;
		case PROGRESS_WRITENONROOTMODELS: progressString = "Writing non-root models"; break;
		case PROGRESS_WRITEATTACHMENTS: progressString = "Writing attachments"; break;
		case PROGRESS_WRITECONTENTTYPES: progressString = "Writing content types"; break;
		case PROGRESS_WRITEOBJECTS: progressString = "Writing objects"; break;
		case PROGRESS_WRITENODES: progressString = "Writing Nodes"; break;
		case PROGRESS_WRITETRIANGLES: progressString = "Writing triangles"; break;
		case PROGRESS_WRITESLICES: progressString = "Writing slices"; break;
		default: progressString = "Unknown Progress Identifier";
	}
}
