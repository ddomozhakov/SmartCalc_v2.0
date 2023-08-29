#ifndef CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_VIEW_VIEW_H_
#define CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_VIEW_VIEW_H_

#include <QDateTimeEdit>
#include <QFile>
#include <QLatin1String>
#include <QMainWindow>

#include "QSpinBox"
#include "controller/controller.h"
#include "ui_view.h"

QT_BEGIN_NAMESPACE
namespace s21 {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 public:
  View(class s21::Controller *c, QWidget *parent = nullptr);
  ~View();

 private:
  s21::Controller *controller_;
  Ui::View *ui_;
  QString formula_str_;
  QVector<double> x_plot_, y_plot_;
  bool expression_error_;
  QVector<QDateEdit *> add_cash_dates_edit_;
  QVector<QDoubleSpinBox *> add_cash_values_edit_;
  QVector<QDateEdit *> sub_cash_dates_edit_;
  QVector<QDoubleSpinBox *> sub_cash_values_edit_;
  int coord_add_cash_;
  int coord_sub_cash_;
  int u_minus_second_par_formula_;
  int u_minus_second_par_str_;
  double xmin_;
  double xmax_;
  double xstep_;

 private slots:
  void FormulaAppendOperands();
  void FormulaAppendOperators();
  void FormulaAppendUnaryMinus();
  void FormulaAppendBrackets();
  void FormulaAppendFunctions();
  void FormulaClear();
  void FormulaEvaluate();
  void FormulaPlot();
  void SetAutoYScale();
  void Backspace();
  void UpSize();
  void DepAddCash();
  void DepSubCash();
  void CreditCalculation();
  void DepositCalculation();
};

#endif  // CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_VIEW_VIEW_H_
