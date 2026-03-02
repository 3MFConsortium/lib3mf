
CIterator
====================================================================================================


.. cpp:class:: Lib3MF::CIterator : public CBase 

	




	.. cpp:function:: bool MoveNext()

		Iterates to the next item in the list.

		:returns: Iterates to the next item in the list.


	.. cpp:function:: bool MovePrevious()

		Iterates to the previous item in the list.

		:returns: Iterates to the previous item in the list.


	.. cpp:function:: Lib3MF_uint64 Count()

		Returns the number of items the iterator captures.

		:returns: returns the number of items the iterator captures.


.. cpp:type:: std::shared_ptr<CIterator> Lib3MF::PIterator

	Shared pointer to CIterator to easily allow reference counting.

