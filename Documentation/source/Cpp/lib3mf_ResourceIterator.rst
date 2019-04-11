
CResourceIterator
====================================================================================================


.. cpp:class:: Lib3MF::CResourceIterator : public CBase 

	




	.. cpp:function:: bool MoveNext()

		Iterates to the next resource in the list.

		:returns: Iterates to the next resource in the list.


	.. cpp:function:: bool MovePrevious()

		Iterates to the previous resource in the list.

		:returns: Iterates to the previous resource in the list.


	.. cpp:function:: PResource GetCurrent()

		Returns the resource the iterator points at.

		:returns: returns the resource instance.


	.. cpp:function:: PResourceIterator Clone()

		Creates a new resource iterator with the same resource list.

		:returns: returns the cloned Iterator instance


.. cpp:type:: std::shared_ptr<CResourceIterator> Lib3MF::PResourceIterator

	Shared pointer to CResourceIterator to easily allow reference counting.

