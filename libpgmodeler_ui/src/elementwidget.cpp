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

#include "elementwidget.h"

ElementWidget::ElementWidget(QWidget *parent) : QWidget(parent)
{
	try
	{
		map<QString, vector<QWidget *> > fields_map;

		warning_frame=nullptr;
		element = nullptr;

		setupUi(this);
		elem_expr_hl=new SyntaxHighlighter(elem_expr_txt, false, true);
		elem_expr_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

		parent_obj=nullptr;
		op_class_sel=new ObjectSelectorWidget(ObjectType::OpClass, true, this);
		collation_sel=new ObjectSelectorWidget(ObjectType::Collation, true, this);
		operator_sel=new ObjectSelectorWidget(ObjectType::Operator, true, this);

		element_grid->addWidget(collation_sel, 3,1,1,2);
		element_grid->addWidget(op_class_sel, 4,1,1,2);
		element_grid->addWidget(operator_sel, 5,1,1,2);

		fields_map[BaseObjectWidget::generateVersionsInterval(BaseObjectWidget::AfterVersion, PgSqlVersions::PgSqlVersion91)].push_back(collation_lbl);
		warning_frame=BaseObjectWidget::generateVersionWarningFrame(fields_map);
		element_grid->addWidget(warning_frame, element_grid->count()+1, 0, 1, 3);
		warning_frame->setParent(this);

		connect(column_rb, SIGNAL(toggled(bool)), this, SLOT(selectElementObject()));
		connect(expression_rb, SIGNAL(toggled(bool)), this, SLOT(selectElementObject()));
		connect(sorting_chk, SIGNAL(toggled(bool)), ascending_rb, SLOT(setEnabled(bool)));
		connect(sorting_chk, SIGNAL(toggled(bool)), descending_rb, SLOT(setEnabled(bool)));
		connect(sorting_chk, SIGNAL(toggled(bool)), nulls_first_chk, SLOT(setEnabled(bool)));

		this->setEnabled(false);
		collation_sel->setVisible(false);
		collation_lbl->setVisible(false);
		operator_sel->setVisible(false);
		operator_lbl->setVisible(false);

		BaseObjectWidget::setRequiredField(operator_sel);
		BaseObjectWidget::setRequiredField(operator_lbl);

		setTabOrder(column_rb, column_cmb);
		setTabOrder(column_cmb, expression_rb);
		setTabOrder(expression_rb, elem_expr_txt);
		setTabOrder(elem_expr_txt, collation_sel);
		setTabOrder(collation_sel, collation_sel->rem_object_tb);
		setTabOrder(collation_sel->rem_object_tb, collation_sel->sel_object_tb);
		setTabOrder(collation_sel->sel_object_tb, op_class_sel);

		setTabOrder(op_class_sel, op_class_sel->rem_object_tb);
		setTabOrder(op_class_sel->rem_object_tb, op_class_sel->sel_object_tb);
		setTabOrder(op_class_sel->sel_object_tb, sorting_chk);

		setTabOrder(sorting_chk, ascending_rb);
		setTabOrder(ascending_rb, descending_rb);
		setTabOrder(descending_rb, nulls_first_chk);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ElementWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, Element *elem)
{
	if(!elem)
		this->setEnabled(false);
	else
	{
		IndexElement *idx_elem = dynamic_cast<IndexElement *>(elem);
		ExcludeElement *exc_elem = dynamic_cast<ExcludeElement *>(elem);
		PartitionKey *part_key = dynamic_cast<PartitionKey *>(elem);
		Column *column = elem->getColumn();

		setAttributes(model, parent_obj);

		if(idx_elem)
			setIndexElement(idx_elem);
		else if(exc_elem)
			setExcludeElement(exc_elem);
		else
			setPartitionKey(part_key);

		if(parent_obj->getObjectType() == ObjectType::Table &&
			 (column || (!column && elem->getExpression().isEmpty())))
		{
			column_rb->setChecked(true);

			if(column)
				column_cmb->setCurrentIndex(column_cmb->findText(column->getName()));
		}
		else
		{
			expression_rb->setChecked(true);
			elem_expr_txt->setPlainText(elem->getExpression());
		}

		if(elem->getSortingAttribute(IndexElement::AscOrder))
			ascending_rb->setChecked(true);
		else
			descending_rb->setChecked(true);

		nulls_first_chk->setChecked(elem->getSortingAttribute(IndexElement::NullsFirst));
		sorting_chk->setChecked(elem->isSortingEnabled());
		op_class_sel->setSelectedObject(elem->getOperatorClass());
		collation_sel->setSelectedObject(elem->getCollation());
		operator_sel->setSelectedObject(elem->getOperator());
	}
}

void ElementWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj)
{
	if(!model || !parent_obj)
	{
		this->setEnabled(false);
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(parent_obj->getObjectType()!=ObjectType::Table &&
					parent_obj->getObjectType()!=ObjectType::View &&
					parent_obj->getObjectType()!=ObjectType::Relationship)
		throw Exception(ErrorCode::OprObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->setEnabled(true);
	this->parent_obj=parent_obj;

	op_class_sel->setModel(model);
	collation_sel->setModel(model);
	operator_sel->setModel(model);

	cols_combo_parent->setVisible(parent_obj->getObjectType() == ObjectType::Table);
	column_rb->setVisible(parent_obj->getObjectType() == ObjectType::Table);
	expression_rb->setChecked(parent_obj->getObjectType() == ObjectType::View);

	if(parent_obj->getObjectType() == ObjectType::Table)
		updateColumnsCombo();
}

void ElementWidget::setIndexElement(IndexElement *elem)
{
	createElement<IndexElement>(elem);
	setWindowTitle(tr("Index element properties"));
	collation_sel->setVisible(true);
	collation_lbl->setVisible(true);
	warning_frame->setVisible(true);
}

void ElementWidget::setExcludeElement(ExcludeElement *elem)
{
	createElement<ExcludeElement>(elem);
	setWindowTitle(tr("Exclude element properties"));
	operator_sel->setVisible(true);
	operator_lbl->setVisible(true);
	warning_frame->setVisible(false);
}

void ElementWidget::setPartitionKey(PartitionKey *elem)
{
	createElement<PartitionKey>(elem);
	setWindowTitle(tr("Partition key properties"));
	collation_sel->setVisible(true);
	collation_lbl->setVisible(true);
	sorting_chk->setVisible(false);
	ascending_rb->setVisible(false);
	descending_rb->setVisible(false);
	nulls_first_chk->setVisible(false);
	warning_frame->setVisible(true);
}

Element *ElementWidget::getElement()
{
	return element;
}

void ElementWidget::applyConfiguration()
{
	element->setSortingEnabled(sorting_chk->isChecked());
	element->setSortingAttribute(IndexElement::NullsFirst, nulls_first_chk->isChecked());
	element->setSortingAttribute(IndexElement::AscOrder, ascending_rb->isChecked());
	element->setOperatorClass(dynamic_cast<OperatorClass *>(op_class_sel->getSelectedObject()));
	element->setCollation(dynamic_cast<Collation *>(collation_sel->getSelectedObject()));
	element->setOperator(dynamic_cast<Operator *>(operator_sel->getSelectedObject()));

	if(expression_rb->isChecked())
		element->setExpression(elem_expr_txt->toPlainText().toUtf8());
	else
		element->setColumn(reinterpret_cast<Column *>(column_cmb->itemData(column_cmb->currentIndex()).value<void *>()));
}

void ElementWidget::updateColumnsCombo()
{
	Table *table = dynamic_cast<Table *>(parent_obj);
	Relationship *rel = dynamic_cast<Relationship *>(parent_obj);
	Column *column=nullptr;
	unsigned i, col_count=0;

	try
	{
		column_cmb->clear();
		column_cmb->setVisible(true);
		column_rb->setVisible(true);

		if(table)
		{
			col_count=table->getColumnCount();
			for(i=0; i < col_count; i++)
			{
				column=table->getColumn(i);
				column_cmb->addItem(column->getName(),
														QVariant::fromValue<void *>(column));
			}
		}
		else if(rel)
		{
			col_count=rel->getAttributeCount();
			for(i=0; i < col_count; i++)
			{
				column=rel->getAttribute(i);
				column_cmb->addItem(column->getName(),
														QVariant::fromValue<void *>(column));
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ElementWidget::selectElementObject()
{
	QObject *obj_sender=sender();

	column_rb->blockSignals(true);
	expression_rb->blockSignals(true);

	if(obj_sender==column_rb)
	{
		elem_expr_txt->clear();
		column_cmb->setEnabled(true);
		expression_rb->setChecked(false);
		column_rb->setChecked(true);
		elem_expr_txt->setEnabled(false);
	}
	else
	{
		column_cmb->setEnabled(false);
		column_rb->setChecked(false);
		expression_rb->setChecked(true);
		elem_expr_txt->setEnabled(true);
	}

	column_rb->blockSignals(false);
	expression_rb->blockSignals(false);
}

template<class Class>
void ElementWidget::createElement(Class *elem)
{
	if(element && !dynamic_cast<Class *>(element))
		delete element;

	if(!element)
		element = new Class;

	*element = *elem;
}
