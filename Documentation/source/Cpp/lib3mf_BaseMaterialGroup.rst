
CBaseMaterialGroup
====================================================================================================


.. cpp:class:: Lib3MF::CBaseMaterialGroup : public CResource 

	The BaseMaterialGroup corresponds to a basematerials-element within a 3MF document




	.. cpp:function:: Lib3MF_uint32 GetCount()

		Retrieves the count of base materials in the material group.

		:returns: returns the count of base materials.


	.. cpp:function:: void GetAllPropertyIDs(std::vector<Lib3MF_uint32> & PropertyIDsBuffer)

		returns all the PropertyIDs of all materials in this group

		:param PropertyIDsBuffer: PropertyID of the material in the material group. 


	.. cpp:function:: Lib3MF_uint32 AddMaterial(const std::string & sName, const sColor & DisplayColor)

		Adds a new material to the material group

		:param sName: new name of the base material. 
		:param DisplayColor: Display color of the material 
		:returns: returns new PropertyID of the new material in the material group.


	.. cpp:function:: void RemoveMaterial(const Lib3MF_uint32 nPropertyID)

		Removes a material from the material group.

		:param nPropertyID: PropertyID of the material in the material group. 


	.. cpp:function:: std::string GetName(const Lib3MF_uint32 nPropertyID)

		Returns the base material's name

		:param nPropertyID: PropertyID of the material in the material group. 
		:returns: returns the name of the base material.


	.. cpp:function:: void SetName(const Lib3MF_uint32 nPropertyID, const std::string & sName)

		Sets a base material's name

		:param nPropertyID: PropertyID of the material in the material group. 
		:param sName: new name of the base material. 


	.. cpp:function:: void SetDisplayColor(const Lib3MF_uint32 nPropertyID, const sColor & TheColor)

		Sets a base material's display color.

		:param nPropertyID: PropertyID of the material in the material group. 
		:param TheColor: The base material's display color 


	.. cpp:function:: sColor GetDisplayColor(const Lib3MF_uint32 nPropertyID)

		Returns a base material's display color.

		:param nPropertyID: PropertyID of the material in the material group. 
		:returns: The base material's display color


.. cpp:type:: std::shared_ptr<CBaseMaterialGroup> Lib3MF::PBaseMaterialGroup

	Shared pointer to CBaseMaterialGroup to easily allow reference counting.

