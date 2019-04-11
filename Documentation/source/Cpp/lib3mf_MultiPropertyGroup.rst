
CMultiPropertyGroup
====================================================================================================


.. cpp:class:: Lib3MF::CMultiPropertyGroup : public CResource 

	




	.. cpp:function:: Lib3MF_uint32 GetCount()

		Retrieves the count of MultiProperty-s in the MultiPropertyGroup.

		:returns: returns the count of MultiProperty-s


	.. cpp:function:: void GetAllPropertyIDs(std::vector<Lib3MF_uint32> & PropertyIDsBuffer)

		returns all the PropertyIDs of all MultiProperty-s in this MultiPropertyGroup

		:param PropertyIDsBuffer: PropertyID of the MultiProperty-s in the MultiPropertyGroup. 


	.. cpp:function:: Lib3MF_uint32 AddMultiProperty(const CInputVector<Lib3MF_uint32> & PropertyIDsBuffer)

		Adds a new MultiProperty to the MultiPropertyGroup.

		:param PropertyIDsBuffer: The PropertyIDs of the new MultiProperty. 
		:returns: returns the PropertyID of the new MultiProperty in the MultiPropertyGroup.


	.. cpp:function:: void SetMultiProperty(const Lib3MF_uint32 nPropertyID, const CInputVector<Lib3MF_uint32> & PropertyIDsBuffer)

		Sets the PropertyIDs of a MultiProperty.

		:param nPropertyID: the PropertyID of the MultiProperty to be changed. 
		:param PropertyIDsBuffer: The new PropertyIDs of the MultiProperty 


	.. cpp:function:: void GetMultiProperty(const Lib3MF_uint32 nPropertyID, std::vector<Lib3MF_uint32> & PropertyIDsBuffer)

		Obtains the PropertyIDs of a MultiProperty.

		:param nPropertyID: the PropertyID of the MultiProperty to be queried. 
		:param PropertyIDsBuffer: The PropertyIDs of the MultiProperty 


	.. cpp:function:: void RemoveMultiProperty(const Lib3MF_uint32 nPropertyID)

		Removes a MultiProperty from this MultiPropertyGroup.

		:param nPropertyID: the PropertyID of the MultiProperty to be removed. 


	.. cpp:function:: Lib3MF_uint32 GetLayerCount()

		Retrieves the number of layers of this MultiPropertyGroup.

		:returns: returns the number of layers


	.. cpp:function:: Lib3MF_uint32 AddLayer(const sMultiPropertyLayer & TheLayer)

		Adds a MultiPropertyLayer to this MultiPropertyGroup.

		:param TheLayer: The MultiPropertyLayer to add to this MultiPropertyGroup 
		:returns: returns the index of this MultiPropertyLayer


	.. cpp:function:: sMultiPropertyLayer GetLayer(const Lib3MF_uint32 nLayerIndex)

		Obtains a MultiPropertyLayer of this MultiPropertyGroup.

		:param nLayerIndex: The Index of the MultiPropertyLayer queried 
		:returns: The MultiPropertyLayer with index LayerIndex within MultiPropertyGroup


	.. cpp:function:: void RemoveLayer(const Lib3MF_uint32 nLayerIndex)

		Removes a MultiPropertyLayer from this MultiPropertyGroup.

		:param nLayerIndex: The Index of the MultiPropertyLayer to be removed 


.. cpp:type:: std::shared_ptr<CMultiPropertyGroup> Lib3MF::PMultiPropertyGroup

	Shared pointer to CMultiPropertyGroup to easily allow reference counting.

