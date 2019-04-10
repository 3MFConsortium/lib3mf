
CColorGroup
====================================================================================================


.. cpp:class:: Lib3MF::CColorGroup : public CResource 

	




	.. cpp:function:: Lib3MF_uint32 GetCount()

		Retrieves the count of base materials in this Color Group.

		:returns: returns the count of colors within this color group.


	.. cpp:function:: void GetAllPropertyIDs(std::vector<Lib3MF_uint32> & PropertyIDsBuffer)

		returns all the PropertyIDs of all colors within this group

		:param PropertyIDsBuffer: PropertyID of the color in the color group. 


	.. cpp:function:: Lib3MF_uint32 AddColor(const sColor & TheColor)

		Adds a new value.

		:param TheColor: The new color 
		:returns: PropertyID of the new color within this color group.


	.. cpp:function:: void RemoveColor(const Lib3MF_uint32 nPropertyID)

		Removes a color from the color group.

		:param nPropertyID: PropertyID of the color to be removed from the color group. 


	.. cpp:function:: void SetColor(const Lib3MF_uint32 nPropertyID, const sColor & TheColor)

		Sets a color value.

		:param nPropertyID: PropertyID of a color within this color group. 
		:param TheColor: The color 


	.. cpp:function:: sColor GetColor(const Lib3MF_uint32 nPropertyID)

		Sets a color value.

		:param nPropertyID: PropertyID of a color within this color group. 
		:returns: The color


.. cpp:type:: std::shared_ptr<CColorGroup> Lib3MF::PColorGroup

	Shared pointer to CColorGroup to easily allow reference counting.

