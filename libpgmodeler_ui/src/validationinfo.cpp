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

#include "validationinfo.h"

ValidationInfo::ValidationInfo()
{
	object=nullptr;
	val_type=ValidationAborted;
}

ValidationInfo::ValidationInfo(unsigned val_type, BaseObject *object, vector<BaseObject *> references)
{
	if(val_type >= SqlValidationError)
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if((val_type==NoUniqueName || val_type==BrokenReference) &&
			(!object || references.empty()))
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->val_type=val_type;
	this->object=object;
	this->references=references;
}

ValidationInfo::ValidationInfo(Exception e)
{
	vector<Exception> err_list;

	val_type=SqlValidationError;
	e.getExceptionsList(err_list);

	while(!err_list.empty())
	{
		errors.push_back(err_list.back().getErrorMessage());
		err_list.pop_back();
	}

	errors.removeDuplicates();
}

ValidationInfo::ValidationInfo(const QString &msg)
{
	val_type=ValidationAborted;
	errors.push_back(msg);
}

unsigned ValidationInfo::getValidationType()
{
	return val_type;
}

BaseObject *ValidationInfo::getObject()
{
	return object;
}

vector<BaseObject *> ValidationInfo::getReferences()
{
	return references;
}

QStringList ValidationInfo::getErrors()
{
	return errors;
}

bool ValidationInfo::isValid()
{
	return (((val_type==NoUniqueName || val_type==BrokenReference) && object) ||
					(val_type==SqlValidationError && !errors.empty()));
}
