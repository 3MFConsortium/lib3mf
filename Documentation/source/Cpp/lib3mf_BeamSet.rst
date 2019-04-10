
CBeamSet
====================================================================================================


.. cpp:class:: Lib3MF::CBeamSet : public CBase 

	




	.. cpp:function:: void SetName(const std::string & sName)

		Sets a beamset's name string

		:param sName: new name of the beamset. 


	.. cpp:function:: std::string GetName()

		Retrieves a beamset's name string

		:returns: returns the name of the beamset.


	.. cpp:function:: void SetIdentifier(const std::string & sIdentifier)

		Sets a beamset's identifier string

		:param sIdentifier: new name of the beamset. 


	.. cpp:function:: std::string GetIdentifier()

		Retrieves a beamset's identifier string

		:returns: returns the identifier of the beamset.


	.. cpp:function:: Lib3MF_uint32 GetReferenceCount()

		Retrieves the reference count of a beamset

		:returns: returns the reference count


	.. cpp:function:: void SetReferences(const CInputVector<Lib3MF_uint32> & ReferencesBuffer)

		Sets the references of a beamset

		:param ReferencesBuffer: the new indices of all beams in this beamset 


	.. cpp:function:: void GetReferences(std::vector<Lib3MF_uint32> & ReferencesBuffer)

		Retrieves the references of a beamset

		:param ReferencesBuffer: retrieves the indices of all beams in this beamset 


.. cpp:type:: std::shared_ptr<CBeamSet> Lib3MF::PBeamSet

	Shared pointer to CBeamSet to easily allow reference counting.

