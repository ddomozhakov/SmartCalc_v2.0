#include "model/model.h"

#include <gtest/gtest.h>

TEST(main, normal) {
  std::vector<std::string> infix_strings_1 = {"P5s10p", "1a2",     "2a2",
                                              "1au2a3", "2a2s2",   "7",
                                              "u3a1",   "123i321", "1e1a1.0"};
  std::vector<double> expected_results_1 = {-5.0, 3.0,  4.0,     2.0, 2.0,
                                            7.0,  -2.0, 123.321, 11};
  auto exp_res_iter = expected_results_1.begin();
  s21::Model model;
  for (auto it = infix_strings_1.begin(); it < infix_strings_1.end(); ++it) {
    bool error = 0;
    model.Compute(*it, 0, &error);
    EXPECT_DOUBLE_EQ(model.GetData(), *exp_res_iter);
    ++exp_res_iter;
  }

  std::vector<std::string> infix_strings_2 = {
      "C(1)ac(1)s3a2m(s2)a3w(3)sg(10)", "C(1)ac(1)s3mu2m(30o2)aC(1)",
      "t(2)mR(0.3)sr(T(0.1))mq(144)dl(3)"};
  std::vector<double> expected_results_2 = {
      sin(1) + cos(1) - 3 + 2 * (-2) + pow(3, 3) - log10(10),
      sin(1) + cos(1) - 3 * (-2) * (fmod(30, 2)) + sin(1),
      tan(2) * acos(0.3) - asin(atan(0.1)) * sqrt(144) / log(3)};
  exp_res_iter = expected_results_2.begin();
  for (auto it = infix_strings_2.begin(); it < infix_strings_2.end(); ++it) {
    bool error = 0;
    model.Compute(*it, 0, &error);
    EXPECT_DOUBLE_EQ(model.GetData(), *exp_res_iter);
    ++exp_res_iter;
  }

  std::string infix_string = "C1a1";
  double expected_result = sin(1) + 1;
  bool error = 0;
  model.Compute(infix_string, 0, &error);
  EXPECT_DOUBLE_EQ(model.GetData(), expected_result);
}

TEST(main, not_normal) {
  std::string infix_string_1 = "1a";
  bool error = 0;
  s21::Model model;
  model.Compute(infix_string_1, 0, &error);
  EXPECT_EQ(error, true);

  std::string infix_string_2 = "(((1a2))";
  error = 0;
  model.Compute(infix_string_2, 0, &error);
  EXPECT_EQ(error, true);

  std::string infix_string_3 = "CCC";
  model.Compute(infix_string_3, 0, &error);
  EXPECT_EQ(error, true);

  std::string infix_string_4 = "^Cc";
  model.Compute(infix_string_4, 0, &error);
  EXPECT_EQ(error, true);

  std::string infix_string_5 = "^3";
  model.Compute(infix_string_5, 0, &error);
  EXPECT_EQ(error, true);
}

TEST(main, credits_1) {
  s21::Model model;
  double loan_amount = 1000, interest_rate = 10, loan_term_in_months = 12,
         monthly_payment, common_payment, overpayment;

  model.AnnuityLoan(loan_amount, interest_rate, loan_term_in_months,
                    monthly_payment, common_payment, overpayment);
  EXPECT_DOUBLE_EQ(monthly_payment, 87.91);
  EXPECT_DOUBLE_EQ(common_payment, 1054.99);
  EXPECT_DOUBLE_EQ(overpayment, 54.99);
}

TEST(main, credits_2) {
  double loan_amount = 1000, interest_rate = 10, loan_term_in_months = 12;
  s21::Model model;
  std::string debt_interest = "";
  std::string monthly_payment = "";
  std::string interest_charges = "";
  std::string debt_interest_eq = "1054.17";
  std::string monthly_payment_eq = "91.67...84.03";
  std::string interest_charges_eq = "54.17";
  model.DifferentialLoan(loan_amount, interest_rate, loan_term_in_months,
                         debt_interest, monthly_payment, interest_charges);
  EXPECT_EQ(debt_interest == debt_interest_eq, true);
  EXPECT_EQ(monthly_payment == monthly_payment_eq, true);
  EXPECT_EQ(interest_charges == interest_charges_eq, true);
}

TEST(main, deposits) {
  s21::Model model;
  std::vector<unsigned> add_cash_days_std = {};
  std::vector<unsigned> sub_cash_days_std = {};
  std::vector<double> add_cash_values_std = {};
  std::vector<double> sub_cash_values_std = {};
  double dep_proc = 5.0, nalog_proc = 0.0, dep_sum = 1000, profit = 0.0,
         nalog = 0.0, result = 0.0;
  bool no_capitalization = false;

  model.DepCalc(0, 365, add_cash_days_std, sub_cash_days_std,
                add_cash_values_std, sub_cash_values_std, dep_proc, nalog_proc,
                dep_sum, no_capitalization, profit, nalog, result);
  EXPECT_DOUBLE_EQ(profit, 51.26);
  EXPECT_DOUBLE_EQ(result, 1102.53);
  EXPECT_DOUBLE_EQ(nalog, 0);

  nalog_proc = 13;
  model.DepCalc(0, 365, add_cash_days_std, sub_cash_days_std,
                add_cash_values_std, sub_cash_values_std, dep_proc, nalog_proc,
                dep_sum, no_capitalization, profit, nalog, result);
  EXPECT_DOUBLE_EQ(profit, 44.60);
  EXPECT_DOUBLE_EQ(result, 1095.87);
  EXPECT_DOUBLE_EQ(nalog, 6.66);

  no_capitalization = true;
  model.DepCalc(0, 365, add_cash_days_std, sub_cash_days_std,
                add_cash_values_std, sub_cash_values_std, dep_proc, nalog_proc,
                dep_sum, no_capitalization, profit, nalog, result);
  EXPECT_DOUBLE_EQ(profit, 43.50);
  EXPECT_DOUBLE_EQ(result, 1043.50);
  EXPECT_DOUBLE_EQ(nalog, 6.5);

  add_cash_days_std = {1};
  sub_cash_days_std = {2};
  add_cash_values_std = {100};
  sub_cash_values_std = {500};

  model.DepCalc(0, 365, add_cash_days_std, sub_cash_days_std,
                add_cash_values_std, sub_cash_values_std, dep_proc, nalog_proc,
                dep_sum, no_capitalization, profit, nalog, result);
  EXPECT_DOUBLE_EQ(profit, 26.20);
  EXPECT_DOUBLE_EQ(result, 626.20);
  EXPECT_DOUBLE_EQ(nalog, 3.91);
}

TEST(main, plot) {
  s21::Model model;
  std::string infix_string = "x";
  std::vector<double> std_x_plot;
  std::vector<double> std_y_plot;
  double xmin = -10.0, xmax = 10.0, xstep = 1;
  bool error = false;
  model.Plot(infix_string, std_x_plot, std_y_plot, xmin, xmax, xstep, error);
  EXPECT_EQ(std_x_plot.size(), 21);
  EXPECT_DOUBLE_EQ(std_y_plot[10], 0);
}
