/*++

Copyright (C) 2018 Autodesk Inc.

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

#include <cmath>
#include <algorithm>
#include <limits>



NMR::CProgressMonitor::CProgressMonitor()
{
	m_progressCallback = nullptr;
	m_userData = nullptr;
	m_lastCallbackResult = true;
}

bool NMR::CProgressMonitor::QueryCancelled()
{
	return Progress(-1, ProgressIdentifier::PROGRESS_QUERYCANCELED);
}

bool NMR::CProgressMonitor::Progress(double progress, ProgressIdentifier identifier)
{
	if (m_progressCallback)
	{
		std::unique_lock<std::mutex> lock(m_callbackMutex, std::try_to_lock);
		if (lock) // If another progress callback is happening right _now_, just drop this one
		{
			int nProgress;
			if (progress == -1)
				nProgress = -1;
			else {
				nProgress = int(100*(Level().first + std::max(std::min(progress, 1.0), 0.0) * (Level().second - Level().first)));
			}
			m_lastCallbackResult = m_progressCallback(nProgress, identifier, m_userData);
			if(m_lastCallbackResult == false)
				return m_lastCallbackResult;
			return m_lastCallbackResult;
		}
	}
	return true;
}

void NMR::CProgressMonitor::PushLevel(double relativeStart, double relativeEnd) {
	std::pair<double, double> curLevel = Level();
	double curRange = curLevel.second - curLevel.first;
	m_levels.push(std::pair<double, double>(curLevel.first + curRange*relativeStart, curLevel.first + curRange*relativeEnd));
}

std::pair<double, double> NMR::CProgressMonitor::PopLevel() {
	std::pair<double, double> ret = Level();
	if (!m_levels.empty()) {
		m_levels.pop();
	}
	return ret;
}

void NMR::CProgressMonitor::ResetLevels() {
	while (!m_levels.empty())
		m_levels.pop();
}

std::pair<double, double> NMR::CProgressMonitor::Level() {
	if (m_levels.empty()) {
		m_levels.push(std::pair<double, double>(0., 1.));
	}
	return m_levels.top();
}

void NMR::CProgressMonitor::SetProgressCallback(Lib3MFProgressCallback callback, void * userData)
{
	m_progressCallback = callback;
	m_userData = userData;
	m_lastCallbackResult = true;
	ResetLevels();
}

void NMR::CProgressMonitor::ClearProgressCallback()
{
	SetProgressCallback(nullptr, nullptr);
}

bool NMR::CProgressMonitor::WasAborted()
{
	return (m_lastCallbackResult == false);
}

void NMR::CProgressMonitor::GetProgressMessage(NMR::ProgressIdentifier progressIdentifier, const char ** progressString) {
	switch (progressIdentifier) {
		case PROGRESS_QUERYCANCELED: *progressString = ""; break;
		case PROGRESS_DONE: *progressString = "Done"; break;
		case PROGRESS_CLEANUP: *progressString = "Cleaning up"; break;
		case PROGRESS_READSTREAM: *progressString = "Reading stream"; break;
		case PROGRESS_EXTRACTOPCPACKAGE: *progressString = "Extracting OPC package"; break;
		case PROGRESS_READNONROOTMODELS: *progressString = "Reading non-root models"; break;
		case PROGRESS_READROOTMODEL: *progressString = "Reading root model"; break;
		case PROGRESS_READRESOURCES: *progressString = "Reading resources"; break;
		case PROGRESS_READMESH: *progressString = "Reading mesh data"; break;
		case PROGRESS_READSLICES: *progressString = "Reading slice data"; break;
		case PROGRESS_READBUILD: *progressString = "Reading build definition"; break;
		case PROGRESS_CREATEOPCPACKAGE: *progressString = "Creating OPC package"; break;
		case PROGRESS_WRITEMODELSTOSTREAM: *progressString = "Writing models to stream"; break;
		case PROGRESS_WRITEROOTMODEL: *progressString = "Writing root model"; break;
		case PROGRESS_WRITENONROOTMODELS: *progressString = "Writing non-root models"; break;
		case PROGRESS_WRITEATTACHMENTS: *progressString = "Writing attachments"; break;
		case PROGRESS_WRITECONTENTTYPES: *progressString = "Writing content types"; break;
		case PROGRESS_WRITENOBJECTS: *progressString = "Writing objects"; break;
		case PROGRESS_WRITENODES: *progressString = "Writing Nodes"; break;
		case PROGRESS_WRITETRIANGLES: *progressString = "Writing triangles"; break;
		case PROGRESS_WRITESLICES: *progressString = "Writing slices"; break;
		default: *progressString = "Unknown Progress Identifier";
	}
}
