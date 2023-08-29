#ifndef CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_CONTROLLER_CONTROLLER_H_
#define CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_CONTROLLER_CONTROLLER_H_

#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QString>
#include <QVector>
#include <vector>

#include "../model/model.h"

namespace s21 {
class Controller {
 public:
  Controller(class s21::Model *m) : model_(m){};
  void Compute(QString &input_expression, double in_double, bool *error);
  void Plot(QString &input_expression, QVector<double> &x_plot,
            QVector<double> &y_plot, double xmin, double xmax, double xstep,
            bool &error);
  double GetData();
  void AnnuityLoan(double loan_amount, double interest_rate,
                   int loan_term_in_months, double *monthly_payment,
                   double *common_payment, double *overpayment);
  void DifferentialLoan(double loan_amount, double interest_rate,
                        int loan_term_in_months, QString &debt_interest,
                        QString &monthly_payment, QString &interest_charges);
  void DepCalc(bool no_capitalization, double &profit,
               QVector<QDateEdit *> add_cash_dates,
               QVector<QDoubleSpinBox *> add_cash_values,
               QVector<QDateEdit *> sub_cash_dates,
               QVector<QDoubleSpinBox *> sub_cash_values, QDate start_date,
               double dep_term, double dep_proc, double nalog_proc,
               double dep_sum, double &nalog, double &result);

 private:
  s21::Model *model_;
  std::string QstrToStr(QString &qstr);
};
}  // namespace s21
#endif  // CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_CONTROLLER_CONTROLLER_H_
