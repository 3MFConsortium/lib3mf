.. Lib3MF documentation master file

Lib3MF v2.0.0-alpha documentation
================================================

.. image:: https://img.shields.io/static/v1.svg?label=license&message=BSD&color=green
	:target: source/license.html
	:alt: Simplified BSD License
	
.. image:: https://travis-ci.org/3MFConsortium/lib3mf.svg?branch=master
	:target: https://travis-ci.org/3MFConsortium/lib3mf
	:alt: Build Status

.. image:: https://readthedocs.org/projects/lib3mf-docs/badge/?version=latest
	:target: https://readthedocs.org/projects/lib3mf-docs/
	:alt: Documentation Status

.. image:: https://img.shields.io/static/v1.svg?label=lib3mf&message=v2.0.0-alpha&color=orange
	:alt: Version

.. image:: https://img.shields.io/static/v1.svg?label=platform&message=windows%20%7C%20macos%20%7C%20linux&color=lightgrey
	:alt: Supported platforms


Welcome! This is the documentation for Lib3MF v2.0.0-alpha.

Lib3MF is an implementation of the 3D Manufacturing Format file standard that provides 
language bindings for a number of programming languages:

.. _label-api-documentation:

API documentation
-----------------

.. toctree::
	:maxdepth: 1
	
	source/Cpp/LandingPage
	source/C/LandingPage
	source/CSharp/LandingPage
	source/Python/LandingPage
	source/Pascal/LandingPage
	source/Golang/LandingPage
	source/NodeJS/LandingPage
	


Obtaining Lib3MF
----------------------------------
Simply download the precompiled binary SDK https://github.com/3MFConsortium/lib3mf/releases.

On Unix-systems you have to install `uuid <https://linux.die.net/man/3/uuid>`_
    - Linux (Ubuntu/Debian/...): <sudo apt-get install uuid-dev>
    - MAC: <brew install ossp-uuid>


Using Lib3MF
-----------------
Allthough the different language bindings are kept as similar as possible,
the usage of Lib3MF still depends your programming language.
You are best of starting with one of the examples distributed in the SDK.

In addition, the home pages for each language binding give detailed instructions on how to use them.

	
Meta Information
-----------------

:doc:`license`

`Reporting Bugs <https://github.com/3MFConsortium/lib3mf/blob/master/CONTRIBUTING.md#filing-issues>`_

`The 3MF Consortium <https://3mf.io/>`_

`Specification of the 3MF format <https://3mf.io/specification/>`_

Indices and tables
-------------------

* :ref:`genindex`
* :ref:`search`
