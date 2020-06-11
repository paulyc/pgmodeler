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
\ingroup libpgmodeler_ui
\class SchemaWidget
\brief Implements the operations to create/edit schemas via form.
*/

#ifndef SCHEMA_WIDGET_H
#define SCHEMA_WIDGET_H

#include "baseobjectwidget.h"
#include "colorpickerwidget.h"
#include "ui_schemawidget.h"

class SchemaWidget: public BaseObjectWidget, public Ui::SchemaWidget {
	private:
		Q_OBJECT

		ColorPickerWidget *color_picker;

	public:
		SchemaWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema);

	public slots:
		void applyConfiguration();
};

#endif
