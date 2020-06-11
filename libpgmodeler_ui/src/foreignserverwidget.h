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
\class ForeignServerWidget
\brief Implements the operations to create/edit foreign servers via form.
*/

#ifndef FOREIGN_SERVER_WIDGET_H
#define FOREIGN_SERVER_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_foreignserverwidget.h"
#include "objectstablewidget.h"

class ForeignServerWidget: public BaseObjectWidget, public Ui::ForeignServerWidget {
	private:
		Q_OBJECT

		ObjectSelectorWidget *fdw_sel;

		ObjectsTableWidget *options_tab;

	public:
		ForeignServerWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, ForeignServer *server);

	public slots:
		void applyConfiguration();
};

#endif
