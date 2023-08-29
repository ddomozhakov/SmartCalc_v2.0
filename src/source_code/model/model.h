#ifndef CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_MODEL_MODEL_H_
#define CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_MODEL_MODEL_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace s21 {
class Model {
 public:
  Model() : data_(0){};
  void Compute(std::string &infix, double in_double, bool *error);
  void Plot(std::string input_expression, std::vector<double> &std_x_plot,
            std::vector<double> &std_y_plot, double xmin, double xmax,
            double xstep, bool &error);
  double GetData() const;
  void AnnuityLoan(double loan_amount, double interest_rate,
                   int loan_term_in_months, double &monthly_payment,
                   double &common_payment, double &overpayment);
  void DifferentialLoan(double loan_amount, double interest_rate,
                        int loan_term_in_months, std::string &debt_interest,
                        std::string &monthly_payment,
                        std::string &interest_charges);
  void MoneyTransactions(std::vector<unsigned> &add_cash_days_std,
                         std::vector<unsigned> &sub_cash_days_std,
                         std::vector<double> &add_cash_values_std,
                         std::vector<double> &sub_cash_values_std,
                         unsigned current_day, double &dep_sum);
  void DepCalc(unsigned start_day, unsigned end_day,
               std::vector<unsigned> add_cash_days_std,
               std::vector<unsigned> sub_cash_days_std,
               std::vector<double> add_cash_values_std,
               std::vector<double> sub_cash_values_std, double dep_proc,
               double nalog_proc, double dep_sum, bool no_capitalization,
               double &profit, double &nalog, double &result);

 private:
  typedef struct CalcNode {
    char sym;
    double val;
    struct CalcNode *next;
  } CalcNode;
  double data_;
  void Push(CalcNode **head, char in_char, double in_val);
  int Pop(CalcNode **head, char *sym, double *val);
  int GetPrior(char sym);
  int AriphPrior(CalcNode *stack, char new_sym);
  void PofixAdd(std::string &pofics, int *pofix_pointer, char sym);
  void AddVSlash(std::string &pofics, int *pofix_pointer, int *is_digit);
  void FreeList(CalcNode *head);
  void ParenthesesChecker(bool *infics_error, std::string infix);
  void ParenthesesAdder(bool *infics_error, CalcNode *stack,
                        std::string &pofics, int *pofix_pointer,
                        std::string infix, int *is_digit, int *i,
                        char *pop_char, double *pop_val);
  void UnaryChecker(std::string &infix);
  void DeleteUnderscore(std::string &infix_with_);
  void Exponenta(std::string &infix);
  void InPolish(std::string infix_with_, std::string &pofics,
                bool *infics_error);
  void DoublePop(CalcNode **stack, double *a, double *b, char *curr_char,
                 bool *pofix_error);
  void SinglePop(CalcNode **stack, double *a, char *curr_char,
                 bool *pofix_error);
  double AddCalc(CalcNode **stack, bool *pofix_error);
  double SubCalc(CalcNode **stack, bool *pofix_error);
  double MultCalc(CalcNode **stack, bool *pofix_error);
  double DivCalc(CalcNode **stack, bool *pofix_error);
  double PowCalc(CalcNode **stack, bool *pofix_error);
  double ModCalc(CalcNode **stack, bool *pofix_error);
  double UMCalc(CalcNode **stack, bool *pofix_error);
  double CosCalc(CalcNode **stack, bool *pofix_error);
  double SinCalc(CalcNode **stack, bool *pofix_error);
  double TanCalc(CalcNode **stack, bool *pofix_error);
  double AcosCalc(CalcNode **stack, bool *pofix_error);
  double AsinCalc(CalcNode **stack, bool *pofix_error);
  double AtanCalc(CalcNode **stack, bool *pofix_error);
  double SqrtCalc(CalcNode **stack, bool *pofix_error);
  double LogCalc(CalcNode **stack, bool *pofix_error);
  double LnCalc(CalcNode **stack, bool *pofix_error);
  double StepCalc(CalcNode **stack, char oper, bool *pofix_error);
  double FromPolish(const std::string &pofics, double in_double,
                    bool *pofix_error);
  double HundredRound(double val);
  double RoundBankers(double val);
};
}  // namespace s21
#endif  // CPP3_S21_SMART_CALC_V2_0_SRC_SOURCE_CODE_MODEL_MODEL_H_
