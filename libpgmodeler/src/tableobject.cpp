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

#include "tableobject.h"

TableObject::TableObject()
{
	parent_table=nullptr;
	decl_in_table=true;
	add_by_linking=add_by_generalization=add_by_copy=false;
}

void TableObject::setParentTable(BaseTable *table)
{
	parent_table=table;
}

BaseTable *TableObject::getParentTable()
{
	return parent_table;
}

void TableObject::setAddedByLinking(bool value)
{
	add_by_linking=value;
	add_by_generalization=false;
	add_by_copy=false;
}

bool TableObject::isAddedByLinking()
{
	return add_by_linking;
}

void TableObject::setAddedByGeneralization(bool value)
{
	add_by_generalization=value;
	add_by_linking=false;
	add_by_copy=false;
}

void TableObject::setDeclaredInTable(bool value)
{
	setCodeInvalidated(decl_in_table != value);
	decl_in_table=value;
}

bool TableObject::isAddedByGeneralization()
{
	return add_by_generalization;
}

void TableObject::setAddedByCopy(bool value)
{
	add_by_copy=value;
	add_by_generalization=false;
	add_by_linking=false;
}

bool TableObject::isAddedByCopy()
{
	return add_by_copy;
}

bool TableObject::isAddedByRelationship()
{
	return (add_by_linking || add_by_generalization || add_by_copy);
}

bool  TableObject::isDeclaredInTable()
{
	return decl_in_table;
}

bool TableObject::isTableObject(ObjectType type)
{
	return (type==ObjectType::Column || type==ObjectType::Constraint || type==ObjectType::Trigger ||
			 type==ObjectType::Rule || type==ObjectType::Index || type==ObjectType::Policy);
}

void TableObject::operator = (TableObject &object)
{
	*(dynamic_cast<BaseObject *>(this))=dynamic_cast<BaseObject &>(object);
	this->parent_table=object.parent_table;
	this->add_by_copy=false;
	this->add_by_generalization=false;
	this->add_by_linking=false;
	this->decl_in_table=object.decl_in_table;
}

void TableObject::setCodeInvalidated(bool value)
{
	if(parent_table)
		parent_table->BaseObject::setCodeInvalidated(value);

	BaseObject::setCodeInvalidated(value);
}

QString TableObject::getDropDefinition(bool cascade)
{
	if(getParentTable())
		attributes[Attributes::Table]=getParentTable()->getName(true);

	attributes[this->getSchemaName()]=Attributes::True;

	return BaseObject::getDropDefinition(cascade);
}

QString TableObject::getSignature(bool format)
{
	if(!parent_table)
		return BaseObject::getSignature(format);

	return QString("%1.%2").arg(parent_table->getSignature(format)).arg(this->getName(format));
}
