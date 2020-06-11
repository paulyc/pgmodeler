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
\class ObjectSelectorWidget
\brief Implements a simple object picker based upon the object view widget.
*/

#ifndef OBJECT_SELECTOR_WIDGET_H
#define OBJECT_SELECTOR_WIDGET_H

#include <QtWidgets>
#include "databasemodel.h"
#include "syntaxhighlighter.h"
#include "modelobjectswidget.h"
#include "ui_objectselectorwidget.h"

class ObjectSelectorWidget: public QWidget, public Ui::ObjectSelectorWidget {
	private:
		Q_OBJECT

		//! \brief An object view widget instance used as object picker
		ModelObjectsWidget *obj_view_wgt;

		//! \brief Selected object name highlighter
		SyntaxHighlighter *obj_name_hl;

		//! \brief Selected object reference
		BaseObject *selected_obj;

		//! \brief Object types that the selector is able to pick
		vector<ObjectType> sel_obj_types;

		//! \brief Stores the database model which the selector must search the object
		DatabaseModel *model;

		//! \brief Configures the selectors attributes at construction time
		void configureSelector(bool install_highlighter);

		bool eventFilter(QObject *obj, QEvent *evnt);

	public:
		ObjectSelectorWidget(ObjectType sel_obj_type, bool install_highlighter, QWidget * parent = nullptr);
		ObjectSelectorWidget(vector<ObjectType> sel_obj_types, bool install_highlighter, QWidget * parent = nullptr);
		virtual ~ObjectSelectorWidget();

		//! \brief Enables the creation of new objects from withing the object selector dialog
		void enableObjectCreation(bool value);

		//! \brief Returns the reference to the selected object
		BaseObject *getSelectedObject();

		//! \brief Returns the selected object formated name
		QString getSelectedObjectName();

		//! \brief Defines the initial selected object to be show on the selector
		void setSelectedObject(BaseObject *object);

		//! \brief Defines the initial selected object to be show on the selector by searching it using the obj_name
		void setSelectedObject(const QString &obj_name, ObjectType obj_type);

		//! \brief Defines the model which the selector will search the objects
		void setModel(DatabaseModel *model);

	private slots:
		void showSelectedObject(BaseObject *obj_sel, bool=false);

		//! \brief Shows the object view widget (picker)
		void showObjectView();

	public slots:
		void clearSelector();

	signals:
		//! \brief Signal emitted when the user selects an object
		void s_objectSelected();

		//! \brief Signal emitted when the user clears the selector
		void s_selectorCleared();

		/*! \brief Signal emitted when the user clears the selector or selects and object
		 * The boolean param indicates if there's an object selected or not */
		void s_selectorChanged(bool);
};

#endif
