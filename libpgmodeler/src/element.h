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
\ingroup libpgmodeler
\brief Implements base operations to handle index/constraint(exclude) elements
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include "column.h"
#include "operatorclass.h"
#include "collation.h"

class Element {
	private:
		/*! \brief Column referenced by the  element. This attribute is
		 mutually exclusive with the expression attribute, this means,
		 when one is set the other has empty (null) value */
		Column *column;

		/*! \brief Expression referenced by the  element. This attribute is
		 mutually exclusive with the column attribute, this means
		 when one is set the other has empty (null) value */
		QString expression;

		//! \brief Operator class used by the  element
		OperatorClass *operator_class;

		/*! \brief Sorting attributes of the element (ASC|DESC, NULLS [FIRST|LAST])
		 This attibutes can be configured used the constants ASC_ORDER and nullptrS_FIRST */
		bool sorting_attibs[2],

		//! \brief Enable the use of the sort attributes
		sorting_enabled;

		//! \brief Compares the attributes of provided element against this returning true/false if the match or not
		bool isEqualsTo(Element &elem);

	protected:
		SchemaParser schparser;

		void configureAttributes(attribs_map &attributes, unsigned def_type);

	public:
		//! \brief Constants used to reference the sorting method of the element
		static constexpr unsigned AscOrder=0,
		NullsFirst=1;

		Element();
		virtual ~Element(void) {}

		//! \brief Element configuration methods
		virtual void setColumn(Column *column);
		virtual void setExpression(const QString &expression);
		virtual void setOperatorClass(OperatorClass *oper_class);
		virtual void setCollation(Collation *){}
		virtual void setOperator(Operator *){}
		void setSortingEnabled(bool value);

		//! \brief Sets the state of one of the element sorting method
		void setSortingAttribute(unsigned attrib, bool value);

		//! \brief Gets the curret state of the element sorting attribute
		bool getSortingAttribute(unsigned attrib);

		Column *getColumn();
		QString getExpression();
		OperatorClass *getOperatorClass();
		virtual Collation *getCollation(void){ return nullptr; }
		virtual Operator *getOperator(void){ return nullptr; }

		bool isSortingEnabled();

		virtual QString getCodeDefinition(unsigned) { return ""; }
		bool operator == (Element &elem);
		bool operator ==(const Element &elem);
};

#endif
