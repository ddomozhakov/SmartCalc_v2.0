#include "controller.h"

void s21::Controller::Compute(QString &input_expression, double in_double,
                              bool *error) {
  std::string ch_test_string = QstrToStr(input_expression);
  model_->Compute(ch_test_string, in_double, error);
}

void s21::Controller::Plot(QString &input_expression, QVector<double> &x_plot,
                           QVector<double> &y_plot, double xmin, double xmax,
                           double xstep, bool &error) {
  std::string ch_test_string = QstrToStr(input_expression);
  std::vector<double> std_x_plot;
  std::vector<double> std_y_plot;
  model_->Plot(ch_test_string, std_x_plot, std_y_plot, xmin, xmax, xstep,
               error);
  x_plot = QVector<double>(std_x_plot.begin(), std_x_plot.end());
  y_plot = QVector<double>(std_y_plot.begin(), std_y_plot.end());
}

double s21::Controller::GetData() { return model_->GetData(); }

std::string s21::Controller::QstrToStr(QString &qstr) {
  return qstr.toStdString();
}

void s21::Controller::AnnuityLoan(double loan_amount, double interest_rate,
                                  int loan_term_in_months,
                                  double *monthly_payment,
                                  double *common_payment, double *overpayment) {
  model_->AnnuityLoan(loan_amount, interest_rate, loan_term_in_months,
                      *monthly_payment, *common_payment, *overpayment);
}

void s21::Controller::DifferentialLoan(double loan_amount, double interest_rate,
                                       int loan_term_in_months,
                                       QString &debt_interest,
                                       QString &monthly_payment,
                                       QString &interest_charges) {
  std::string debt_interest_std = QstrToStr(debt_interest);
  std::string monthly_payment_std = QstrToStr(monthly_payment);
  std::string interest_charges_std = QstrToStr(interest_charges);

  model_->DifferentialLoan(loan_amount, interest_rate, loan_term_in_months,
                           debt_interest_std, monthly_payment_std,
                           interest_charges_std);
  debt_interest = QString::fromStdString(debt_interest_std);
  monthly_payment = QString::fromStdString(monthly_payment_std);
  interest_charges = QString::fromStdString(interest_charges_std);
}

void s21::Controller::DepCalc(bool no_capitalization, double &profit,
                              QVector<QDateEdit *> add_cash_dates_edit,
                              QVector<QDoubleSpinBox *> add_cash_values_edit,
                              QVector<QDateEdit *> sub_cash_dates_edit,
                              QVector<QDoubleSpinBox *> sub_cash_values_edit,
                              QDate start_date, double dep_term,
                              double dep_proc, double nalog_proc,
                              double dep_sum, double &nalog, double &result) {
  unsigned start_day = start_date.year() * 365 + start_date.dayOfYear(),
           end_day = start_day + dep_term;

  std::vector<unsigned> add_cash_days_std, sub_cash_days_std;
  std::vector<double> add_cash_values_std, sub_cash_values_std;

  for (int i = 0; i < add_cash_dates_edit.size(); ++i) {
    add_cash_days_std.push_back(add_cash_dates_edit[i]->date().year() * 365 +
                                add_cash_dates_edit[i]->date().dayOfYear());
  }
  for (int i = 0; i < add_cash_values_edit.size(); ++i) {
    add_cash_values_std.push_back(add_cash_values_edit[i]->value());
  }
  for (int i = 0; i < sub_cash_dates_edit.size(); ++i) {
    sub_cash_days_std.push_back(sub_cash_dates_edit[i]->date().year() * 365 +
                                sub_cash_dates_edit[i]->date().dayOfYear());
  }
  for (int i = 0; i < sub_cash_values_edit.size(); ++i) {
    sub_cash_values_std.push_back(sub_cash_values_edit[i]->value());
  }
  model_->DepCalc(start_day, end_day, add_cash_days_std, sub_cash_days_std,
                  add_cash_values_std, sub_cash_values_std, dep_proc,
                  nalog_proc, dep_sum, no_capitalization, profit, nalog,
                  result);
}
