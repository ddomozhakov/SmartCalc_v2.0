#include "view.h"

View::View(class s21::Controller *c, QWidget *parent)
    : controller_(c),
      QMainWindow(parent),
      ui_(new Ui::View),
      expression_error_(false),
      xmin_(-20),
      xmax_(20),
      xstep_(0.01),
      coord_add_cash_(0),
      coord_sub_cash_(0) {
  ui_->setupUi(this);
  ui_->xmax_edit->setValidator(
      new QDoubleValidator(-1000000, 1000000, 100, this));
  ui_->xmin_edit->setValidator(
      new QDoubleValidator(-1000000, 1000000, 100, this));
  ui_->xstep_edit->setValidator(new QDoubleValidator(0, 1000, 100, this));
  ui_->xvalue_edit->setValidator(
      new QDoubleValidator(-1000000, 1000000, 100, this));

  setWindowFlags(windowFlags() & (~Qt::WindowMaximizeButtonHint));

  connect(ui_->push_button_0, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_1, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_2, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_3, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_4, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_5, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_6, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_7, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_8, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_9, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_i, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_x, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));
  connect(ui_->push_button_e, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperands()));

  connect(ui_->push_button_a, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperators()));
  connect(ui_->push_button_s, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperators()));
  connect(ui_->push_button_m, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperators()));
  connect(ui_->push_button_d, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperators()));
  connect(ui_->push_button_o, SIGNAL(clicked()), this,
          SLOT(FormulaAppendOperators()));

  connect(ui_->push_button_u, SIGNAL(clicked()), this,
          SLOT(FormulaAppendUnaryMinus()));

  connect(ui_->push_button_P, SIGNAL(clicked()), this,
          SLOT(FormulaAppendBrackets()));
  connect(ui_->push_button_p, SIGNAL(clicked()), this,
          SLOT(FormulaAppendBrackets()));

  connect(ui_->push_button_cle, SIGNAL(clicked()), this,
          SLOT(FormulaClear()));

  connect(ui_->push_button_R, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_r, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_T, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_c, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_l, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_g, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_w, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_C, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_q, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));
  connect(ui_->push_button_t, SIGNAL(clicked()), this,
          SLOT(FormulaAppendFunctions()));

  connect(ui_->push_button_eq, SIGNAL(clicked()), this,
          SLOT(FormulaEvaluate()));

  connect(ui_->push_button_plo, SIGNAL(clicked()), this,
          SLOT(FormulaPlot()));

  connect(ui_->push_button_as, SIGNAL(clicked()), this,
          SLOT(SetAutoYScale()));

  connect(ui_->push_button_bs, SIGNAL(clicked()), this,
          SLOT(Backspace()));
  // Credit calculator
  connect(ui_->credit_calc, SIGNAL(clicked()), this,
          SLOT(CreditCalculation()));

  //  // Deposit calculator
  connect(ui_->tabWidget, SIGNAL(currentChanged(int)), this,
          SLOT(UpSize()));
  connect(ui_->dep_calc, SIGNAL(clicked()), this,
          SLOT(DepositCalculation()));
  connect(ui_->dep_add_cash, SIGNAL(clicked()), this,
          SLOT(DepAddCash()));
  connect(ui_->dep_sub_cash, SIGNAL(clicked()), this,
          SLOT(DepSubCash()));

  QDate current_date_init = QDate::currentDate();
  ui_->dateEdit->setDate(current_date_init);
  QVBoxLayout *layout = new QVBoxLayout();
  ui_->deposit_tab->setLayout(layout);
}

View::~View() { delete ui_; }

void View::UpSize() {
  if (ui_->tabWidget->currentIndex() == 2) {
    resize(1020, 800);
    ui_->tabWidget->setFixedWidth(1000);
  } else {
    resize(500, 800);
    ui_->tabWidget->setFixedWidth(470);
  }
}

void View::FormulaClear() {
  ui_->formula->setText("0");
  formula_str_ = "";
  ui_->plotter->clearGraphs();
  ui_->plotter->replot();
  x_plot_.clear();
  y_plot_.clear();
}

void View::Backspace() {
  QString current_furmula = ui_->formula->text();
  current_furmula.chop(1);
  ui_->formula->setText(current_furmula);
  formula_str_.chop(1);
}

void View::SetAutoYScale() {
  double y_max = -qInf(), y_min = +qInf();
  if ((!y_plot_.empty()) && (!x_plot_.empty())) {
    for (int i = 0; i < y_plot_.length() - 1; ++i) {
      if (y_plot_[i] > y_max) {
        y_max = y_plot_[i];
      }
      if (y_plot_[i] < y_min) {
        y_min = y_plot_[i];
      }
    }
  }
  ui_->plotter->yAxis->setRange(y_min - 0.1 * y_min, y_max + 0.1 * y_max);
  ui_->plotter->replot();
}

void View::FormulaPlot() {
  x_plot_.clear();
  y_plot_.clear();
  ui_->plotter->clearGraphs();
  ui_->plotter->xAxis->setRange(ui_->xmin_edit->text().toInt(),
                                ui_->xmax_edit->text().toInt());
  ui_->plotter->yAxis->setRange(-10, 10);
  ui_->plotter->xAxis->setLabel("X");
  ui_->plotter->yAxis->setLabel("f(x)");
  if (ui_->xmin_edit->text() != "xmin") {
    xmin_ = ui_->xmin_edit->text().toDouble();
  }
  if (ui_->xmax_edit->text() != "xmax") {
    xmax_ = ui_->xmax_edit->text().toDouble();
  }
  if (ui_->xstep_edit->text() != "xstep") {
    xstep_ = ui_->xstep_edit->text().toDouble();
  }
  controller_->Plot(formula_str_, x_plot_, y_plot_, xmin_, xmax_, xstep_,
                    expression_error_);
  if (expression_error_) {
    ui_->formula->setText("Error");
    expression_error_ = 0;
  }
  double y_diff_average = 0;
  for (int i = 1; i < y_plot_.length() - 1; ++i) {
    if ((y_plot_[i] == y_plot_[i]) && (y_plot_[i - 1] == y_plot_[i - 1])) {
      y_diff_average += abs(y_plot_[i] - y_plot_[i - 1]);
    }
  }
  y_diff_average /= (y_plot_.length() - 1);
  for (int i = 1; i < y_plot_.length() - 1; ++i) {
    if (fabs(y_plot_[i] - y_plot_[i - 1]) > (y_diff_average * 10)) {
      y_plot_[i] = qQNaN();
    }
  }
  ui_->plotter->addGraph();
  ui_->plotter->graph(0)->setLineStyle(QCPGraph::lsLine);
  ui_->plotter->graph(0)->addData(x_plot_, y_plot_);
  ui_->plotter->setInteraction(QCP::iRangeDrag);
  ui_->plotter->setInteraction(QCP::iRangeZoom);
  ui_->plotter->setInteraction(QCP::iSelectPlottables);
  ui_->plotter->setNotAntialiasedElements(QCP::aeAll);
  ui_->plotter->graph(0)->setAdaptiveSampling(false);
  QPen pen;
  pen.setWidth(1);
  ui_->plotter->graph()->setPen(pen);
  ui_->plotter->replot();
}

void View::FormulaEvaluate() {
  double res = 0.0;
  if ((formula_str_.contains("x")) &&
      (ui_->xvalue_edit->text().contains("x"))) {
    ui_->formula->setText("Error: input x value");
  } else {
    controller_->Compute(formula_str_, ui_->xvalue_edit->text().toDouble(),
                         &expression_error_);
    res = controller_->GetData();
    if (!expression_error_) {
      ui_->formula->setText(QString::number(res, 'g', 7));
    } else {
      ui_->formula->setText("Error");
      expression_error_ = 0;
    }
  }
}

void View::FormulaAppendFunctions() {
  QPushButton *fu_button = (QPushButton *)sender();
  if (ui_->formula->text() == "Error") {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  if ((ui_->formula->text() == "0") && (fu_button->text() != "0")) {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  ui_->formula->setText(ui_->formula->text() + " ");
  ui_->formula->setText(ui_->formula->text() + fu_button->text());
  formula_str_.append(" ");
  formula_str_.append(fu_button->objectName().back());
  if (fu_button->objectName().back() == 'w') {
    ui_->formula->setText(ui_->formula->text() + '(');
    formula_str_.append("(");
  }
}

void View::FormulaAppendBrackets() {
  QPushButton *br_button = (QPushButton *)sender();
  if (ui_->formula->text() == "Error") {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  if ((ui_->formula->text() == "0") && (br_button->text() != "0")) {
    ui_->formula->setText("");
  }
  if (br_button->text() == "(") {
    ui_->formula->setText(ui_->formula->text() + " ");
    ui_->formula->setText(ui_->formula->text() + "(");
    ui_->formula->setText(ui_->formula->text() + " ");
    formula_str_.append("(");
  } else {
    ui_->formula->setText(ui_->formula->text() + " ");
    ui_->formula->setText(ui_->formula->text() + ")");
    ui_->formula->setText(ui_->formula->text() + " ");
    formula_str_.append(")");
  }
}

void View::FormulaAppendUnaryMinus() {
  QString formula_ui_ = ui_->formula->text();
  if (ui_->formula->text() == "Error") {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  if (formula_str_.at(0) != 'u') {
    formula_ui_.push_front(' ');
    formula_ui_.push_front('(');
    formula_ui_.push_front(' ');
    formula_ui_.push_front('-');
    formula_ui_.push_back(' ');
    formula_ui_.push_back(')');
    u_minus_second_par_formula_ = formula_ui_.length() - 1;
    ui_->formula->setText(formula_ui_);
    formula_str_.push_front('(');
    formula_str_.push_front('u');
    formula_str_.push_back(')');
    u_minus_second_par_str_ = formula_str_.length() - 1;
  } else if (formula_ui_.at(2) == '(') {
    formula_ui_.remove(u_minus_second_par_formula_ - 1, 2);
    formula_ui_.remove(0, 4);
    formula_str_.remove(u_minus_second_par_str_, 1);
    formula_str_.remove(0, 2);
    ui_->formula->setText(formula_ui_);
  }
}

void View::FormulaAppendOperators() {
  if ((ui_->formula->text() == "Error") || (ui_->formula->text() == "0")) {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  QPushButton *num_button = (QPushButton *)sender();
  if (formula_str_.length() != 0) {
    ui_->formula->setText(ui_->formula->text() + " ");
  }
  formula_str_.append(num_button->objectName().back());
  ui_->formula->setText(ui_->formula->text() + num_button->text());
  ui_->formula->setText(ui_->formula->text() + " ");
}

void View::FormulaAppendOperands() {
  if (ui_->formula->text() == "Error") {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  QPushButton *num_button = (QPushButton *)sender();
  if ((ui_->formula->text() == "0") && (num_button->text() != "0")) {
    ui_->formula->setText("");
    formula_str_ = "";
  }
  if ((ui_->formula->text() != "0") || (num_button->text() != "0")) {
    ui_->formula->setText(ui_->formula->text() + num_button->text());
    formula_str_.append(num_button->objectName().back());
  }
}

void View::CreditCalculation() {
  if (ui_->Ann_opt->isChecked()) {
    double loan_amount = ui_->lineEdit_credit_sum->value();
    double interest_rate = ui_->lineEdit_proc->value();
    int loan_term_in_months = ui_->lineEdit_credit_term->value();
    double monthly_payment, common_payment, overpayment;
    controller_->AnnuityLoan(loan_amount, interest_rate, loan_term_in_months,
                             &monthly_payment, &common_payment, &overpayment);
    ui_->Monthly_pay->setText(QString::number(monthly_payment));
    ui_->Proc->setText(QString::number(overpayment));
    ui_->Debt_and_proc->setText(QString::number(common_payment));
  } else {
    double loan_amount = ui_->lineEdit_credit_sum->value();
    double interest_rate = ui_->lineEdit_proc->value();
    int loan_term_in_months = ui_->lineEdit_credit_term->value();
    QString debt_interest = ui_->Debt_and_proc->text();
    QString monthly_payment = ui_->Monthly_pay->text();
    QString interest_charges = ui_->Proc->text();
    controller_->DifferentialLoan(loan_amount, interest_rate,
                                  loan_term_in_months, debt_interest,
                                  monthly_payment, interest_charges);
    ui_->Monthly_pay->setText(monthly_payment);
    ui_->Proc->setText(interest_charges);
    ui_->Debt_and_proc->setText(debt_interest);
  }
}

void View::DepAddCash() {
  if (coord_add_cash_ <= 700) {
    QDateEdit *dept_add_cash_dates = new QDateEdit();
    dept_add_cash_dates->setDate(QDate::currentDate());
    dept_add_cash_dates->setGeometry(500, 70 + coord_add_cash_, 100, 40);
    QDoubleSpinBox *dept_add_cash_values = new QDoubleSpinBox();
    dept_add_cash_values->setMaximum(1000000000);
    dept_add_cash_values->setGeometry(600, 70 + coord_add_cash_, 100, 40);
    coord_add_cash_ += 50;
    layout()->addWidget(dept_add_cash_dates);
    layout()->addWidget(dept_add_cash_values);
    add_cash_dates_edit_.push_back(dept_add_cash_dates);
    add_cash_values_edit_.push_back(dept_add_cash_values);
  }
}

void View::DepSubCash() {
  if (coord_sub_cash_ <= 700) {
    QDateEdit *dept_sub_cash_dates = new QDateEdit();
    dept_sub_cash_dates->setDate(QDate::currentDate());
    dept_sub_cash_dates->setGeometry(800, 70 + coord_sub_cash_, 100, 40);
    QDoubleSpinBox *dept_sub_cash_values = new QDoubleSpinBox();
    dept_sub_cash_values->setMaximum(1000000000);
    dept_sub_cash_values->setGeometry(910, 70 + coord_sub_cash_, 100, 40);
    coord_sub_cash_ += 50;
    layout()->addWidget(dept_sub_cash_dates);
    layout()->addWidget(dept_sub_cash_values);
    sub_cash_dates_edit_.push_back(dept_sub_cash_dates);
    sub_cash_values_edit_.push_back(dept_sub_cash_values);
  }
}

void View::DepositCalculation() {
  double profit, nalog, result;
  controller_->DepCalc(ui_->dep_nocap->isChecked(), profit,
                       add_cash_dates_edit_, add_cash_values_edit_,
                       sub_cash_dates_edit_, sub_cash_values_edit_,
                       ui_->dateEdit->date(), ui_->dep_term->value() * 31,
                       ui_->dep_proc->value(), ui_->nalog_proc->value(),
                       ui_->dep_sum->value(), nalog, result);
  if (profit < 0) {
    ui_->dep_proc_res->setNum(0);
    ui_->dep_result->setNum(0);
    ui_->dep_nalog_sum->setNum(0);
  } else {
    ui_->dep_proc_res->setNum(profit);
    ui_->dep_result->setNum(result);
    ui_->dep_nalog_sum->setNum(nalog);
  }
}
