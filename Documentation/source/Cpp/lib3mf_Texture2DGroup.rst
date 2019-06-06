
CTexture2DGroup
====================================================================================================


.. cpp:class:: Lib3MF::CTexture2DGroup : public CResource 

	




	.. cpp:function:: Lib3MF_uint32 GetCount()

		Retrieves the count of tex2coords in the Texture2DGroup.

		:returns: returns the count of tex2coords.


	.. cpp:function:: void GetAllPropertyIDs(std::vector<Lib3MF_uint32> & PropertyIDsBuffer)

		returns all the PropertyIDs of all tex2coords in this Texture2DGroup

		:param PropertyIDsBuffer: PropertyID of the tex2coords in the Texture2DGroup. 


	.. cpp:function:: Lib3MF_uint32 AddTex2Coord(const sTex2Coord & UVCoordinate)

		Adds a new tex2coord to the Texture2DGroup

		:param UVCoordinate: The u/v-coordinate within the texture, horizontally right/vertically up from the origin in the lower left of the texture. 
		:returns: returns new PropertyID of the new tex2coord in the Texture2DGroup.


	.. cpp:function:: sTex2Coord GetTex2Coord(const Lib3MF_uint32 nPropertyID)

		Obtains a tex2coord to the Texture2DGroup

		:param nPropertyID: the PropertyID of the tex2coord in the Texture2DGroup. 
		:returns: The u/v-coordinate within the texture, horizontally right/vertically up from the origin in the lower left of the texture.


	.. cpp:function:: void RemoveTex2Coord(const Lib3MF_uint32 nPropertyID)

		Removes a tex2coords from the Texture2DGroup.

		:param nPropertyID: PropertyID of the tex2coords in the Texture2DGroup. 


	.. cpp:function:: PTexture2D GetTexture2D()

		Obtains the texture2D instance of this group.

		:returns: the texture2D instance of this group.


.. cpp:type:: std::shared_ptr<CTexture2DGroup> Lib3MF::PTexture2DGroup

	Shared pointer to CTexture2DGroup to easily allow reference counting.

