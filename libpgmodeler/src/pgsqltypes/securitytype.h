/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libpgmodeler/pgsqltypes
\class SecurityType
\brief Implements the operations to manipulate function's security types.
**/

#ifndef SECURITY_TYPE
#define SECURITY_TYPE

#include "templatetype.h"

class SecurityType: public TemplateType<SecurityType>{
	public:
		static constexpr unsigned Invoker = 1,
		Definer = 2;

		SecurityType(unsigned type_id);
		SecurityType(const QString &type_name);
		SecurityType();
};

#endif 
