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

#include "rulewidget.h"

RuleWidget::RuleWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Rule)
{
	try
	{
		QStringList list;
		QFrame *frame=nullptr;

		Ui_RuleWidget::setupUi(this);

		cond_expr_hl=new SyntaxHighlighter(cond_expr_txt, false, true);
		cond_expr_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

		command_hl=new SyntaxHighlighter(comando_txt, false, true);
		command_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());
		command_cp=new CodeCompletionWidget(comando_txt);

		commands_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^ ObjectsTableWidget::DuplicateButton, true, this);
		commands_tab->setHeaderLabel(tr("SQL command"),0);
		commands_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("codigosql")),0);
		dynamic_cast<QGridLayout *>(commands_gb->layout())->addWidget(commands_tab, 1, 0, 1, 2);

		frame=generateInformationFrame(tr("To create a rule that does not perform any action (<strong>DO NOTHING</strong>) simply do not specify commands in the SQL commands table."));
		rule_grid->addWidget(frame, rule_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		configureFormLayout(rule_grid, ObjectType::Rule);

		event_cmb->addItems(EventType::getTypes());
		exec_type_cmb->addItems(ExecutionType::getTypes());

		connect(commands_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleCommand(int)));
		connect(commands_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleCommand(int)));
		connect(commands_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editCommand(int)));

		setRequiredField(event_lbl);
		configureTabOrder();

		setMinimumSize(550, 500);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RuleWidget::editCommand(int row)
{
	comando_txt->setPlainText(commands_tab->getCellText(row,0));
}

void RuleWidget::handleCommand(int row)
{
	if(!comando_txt->toPlainText().isEmpty())
	{
		commands_tab->setCellText(comando_txt->toPlainText(),row,0);
		comando_txt->clear();
	}
	else if(commands_tab->getCellText(row,0).isEmpty())
		commands_tab->removeRow(row);
}

void RuleWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseTable *parent_tab, Rule *rule)
{
	unsigned qtd, i;

	if(!parent_tab)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, rule, parent_tab);

	command_cp->configureCompletion(model, command_hl);

	if(rule)
	{
		event_cmb->setCurrentIndex(event_cmb->findText(~rule->getEventType()));
		exec_type_cmb->setCurrentIndex(exec_type_cmb->findText(~rule->getExecutionType()));
		cond_expr_txt->setPlainText(rule->getConditionalExpression());

		commands_tab->blockSignals(true);
		qtd=rule->getCommandCount();
		for(i=0; i < qtd; i++)
		{
			commands_tab->addRow();
			commands_tab->setCellText(rule->getCommand(i),i,0);
		}
		commands_tab->blockSignals(false);
	}
}

void RuleWidget::applyConfiguration()
{
	try
	{
		Rule *rule=nullptr;
		unsigned count, i;

		startConfiguration<Rule>();

		rule=dynamic_cast<Rule *>(this->object);
		rule->setEventType(EventType(event_cmb->currentText()));
		rule->setExecutionType(ExecutionType(exec_type_cmb->currentText()));
		rule->setConditionalExpression(cond_expr_txt->toPlainText().toUtf8());
		rule->removeCommands();

		count=commands_tab->getRowCount();

		for(i=0; i < count; i++)
			rule->addCommand(commands_tab->getCellText(i,0).toUtf8());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

