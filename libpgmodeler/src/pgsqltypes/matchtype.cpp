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

#include "matchtype.h"

template<>
QStringList MatchType::TemplateType<MatchType>::type_names =
{
	"", // Reserved for BaseType::null

	"MATCH FULL",
	"MATCH PARTIAL",
	"MATCH SIMPLE"
};

MatchType::MatchType()
{
	type_idx = Full;
}

MatchType::MatchType(const QString &type_name)
{
	setType(type_name);
}

MatchType::MatchType(unsigned type_id)
{
	setType(type_id);
}
