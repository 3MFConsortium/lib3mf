
CVolumeData
====================================================================================================


.. cpp:class:: Lib3MF::CVolumeData : public CResource 

	




	.. cpp:function:: PVolumeDataComposite GetComposite()

		Returns the VolumeDataComposite of this VolumeData instance

		:returns: filled with the VolumeDataComposite of this VolumeData instance.


	.. cpp:function:: PVolumeDataComposite CreateNewComposite()

		Creates a new VolumeDataComposite for this VolumeData instance

		:returns: The new VolumeDataComposite of this VolumeData instance.


	.. cpp:function:: void RemoveComposite()

		Removes the VolumeDataComposite of this VolumeData instance



	.. cpp:function:: PVolumeDataColor GetColor()

		Returns the VolumeDataColor of this VolumeData instance

		:returns: filled with the VolumeDataColor of this VolumeData instance.


	.. cpp:function:: PVolumeDataColor CreateNewColor(classParam<CFunction> pTheFunction)

		Creates a new VolumeDataColor for this VolumeData instance

		:param pTheFunction: Function used in this element 
		:returns: The new VolumeDataColor of this VolumeData instance.


	.. cpp:function:: void RemoveColor()

		Removes the VolumeDataColor of this VolumeData instance



	.. cpp:function:: Lib3MF_uint32 GetPropertyCount()

		Returns the number of VolumeDataProperty

		:returns: the number of VolumeDataProperty-elements within this VolumdeData


	.. cpp:function:: PVolumeDataProperty GetProperty(const Lib3MF_uint32 nIndex)

		Returns the VolumeDataProperty at a given Index

		:param nIndex: the index of the VolumeDataProperty to be returned. 
		:returns: the VolumeDataProperty at the given index.


	.. cpp:function:: PVolumeDataProperty AddPropertyFromFunction(const std::string & sName, classParam<CFunction> pTheFunction)

		Adds a new VolumeDataProperty from a Function

		:param sName: the qualified name (namespace+name) of the Property 
		:param pTheFunction: Function used in this element 
		:returns: the newly created VolumeDataProperty.


	.. cpp:function:: void RemoveProperty(const Lib3MF_uint32 nIndex)

		Removes the VolumeDataProperty with a given index

		:param nIndex: the index of the VolumeDataProperty to be removed. 


.. cpp:type:: std::shared_ptr<CVolumeData> Lib3MF::PVolumeData

	Shared pointer to CVolumeData to easily allow reference counting.

