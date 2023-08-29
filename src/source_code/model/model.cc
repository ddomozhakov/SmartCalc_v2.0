#include "model.h"

void s21::Model::Push(CalcNode **head, char in_char, double in_val) {
  CalcNode *tmp = (CalcNode *)malloc(sizeof(CalcNode));
  tmp->sym = in_char;
  tmp->val = in_val;
  tmp->next = (*head);
  (*head) = tmp;
}

int s21::Model::Pop(CalcNode **head, char *sym, double *val) {
  CalcNode *prev = NULL;
  int error = 0;
  if (head == NULL) {
    error = 1;
  }
  prev = (*head);
  *sym = prev->sym;
  *val = prev->val;
  (*head) = (*head)->next;
  free(prev);
  return error;
}

int s21::Model::GetPrior(char sym) {
  int res = 0;
  std::string prior1 = "cCtRrTqlgu";
  std::string prior2 = "mdwo";
  std::string prior3 = "as";
  if (prior1.find(sym) != std::string::npos) {
    res = 3;
  } else if (prior2.find(sym) != std::string::npos) {
    res = 2;
  } else if (prior3.find(sym) != std::string::npos) {
    res = 1;
  }
  return res;
}

int s21::Model::AriphPrior(CalcNode *stack, char new_sym) {
  int res = 0;
  if (stack) {
    if ((GetPrior(stack->sym) > GetPrior(new_sym))) {
      res = 0;
    } else if (GetPrior(stack->sym) == GetPrior(new_sym)) {
      res = 1;
    } else {
      res = 2;
    }
  } else {
    res = 0;
  }
  if (!stack) {
    res = 2;
  }
  return res;
}

void s21::Model::PofixAdd(std::string &pofics, int *pofix_pointer, char sym) {
  if (((*pofix_pointer) && (pofics[*pofix_pointer - 1] != '|') &&
       (sym == '|')) ||
      (sym != '|')) {
    pofics += sym;
    ++(*pofix_pointer);
  }
}

void s21::Model::AddVSlash(std::string &pofics, int *pofix_pointer,
                           int *is_digit) {
  if (*is_digit) {
    PofixAdd(pofics, pofix_pointer, '|');
    *is_digit = 0;
  }
}

void s21::Model::FreeList(CalcNode *head) {
  CalcNode *tmp;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp);
  }
}

void s21::Model::ParenthesesChecker(bool *infics_error, std::string infix) {
  if (!(*infics_error)) {
    int i = 0;
    int par_counter = 0, par_error = 0;
    while ((infix[i]) && (!(*infics_error))) {
      if (par_counter < 0) {
        par_error = 1;
      }
      if (infix[i] == '(') {
        ++par_counter;
      } else if (infix[i] == ')') {
        --par_counter;
      }
      ++i;
    }
    if (par_counter != 0) {
      par_error = 1;
    }
    if (par_error) {
      *infics_error = 1;
    }
  }
}

void s21::Model::ParenthesesAdder(bool *infics_error, CalcNode *stack,
                                  std::string &pofics, int *pofix_pointer,
                                  std::string infix, int *is_digit, int *i,
                                  char *pop_char, double *pop_val) {
  if (!(*infics_error)) {
    CalcNode *tmp = stack;
    if (tmp) {
      AddVSlash(pofics, pofix_pointer, is_digit);
      CalcNode *nex = stack->next;
      while ((tmp) && (!(*infics_error))) {
        if ((stack->sym == '(') || (stack->sym == ')')) {
          if ((infix[0] != '(') || (infix[*i - 2] != ')')) {
            *infics_error = 1;
          }
        }
        Pop(&stack, pop_char, pop_val);
        if (*pop_char != '(') {
          PofixAdd(pofics, pofix_pointer, *pop_char);
        }
        tmp = nex;
        if (tmp) {
          nex = tmp->next;
        }
      }
    }
    if (*is_digit) {
      pofics[*i] = '|';
    }
  }
}

void s21::Model::UnaryChecker(std::string &infix) {
  std::string unary_callers = "(Pasmdwou";
  if (infix[0] == 's') {
    infix[0] = 'u';
  } else if (infix[0] == 'a') {
    infix[0] = ' ';
  }
  for (auto it = ++infix.begin(); it < infix.end(); ++it) {
    if ((unary_callers.find(*(it - 1)) != std::string::npos) && (*it == 's')) {
      *it = 'u';
    }
  }
  for (auto it = ++infix.begin(); it < infix.end(); ++it) {
    if ((unary_callers.find(*(it - 1)) != std::string::npos) && (*it == 'a')) {
      *it = ' ';
    }
  }
  DeleteUnderscore(infix);
}

void s21::Model::DeleteUnderscore(std::string &infix_with_) {
  infix_with_.erase(remove_if(infix_with_.begin(), infix_with_.end(), isspace),
                    infix_with_.end());
}

void s21::Model::Exponenta(std::string &infix) {
  std::string curr_val_str;
  double curr_val;
  auto before_e = infix.begin(), after_e = infix.begin();
  for (auto it = infix.begin(); it < infix.end(); ++it) {
    if (*it == 'e') {
      before_e = it;
      while (isdigit(*(--before_e)) || (*before_e == 'u') ||
             (*before_e == 's')) {
        ;
      }
      ++before_e;
      after_e = it;
      while (isdigit(*(++after_e)) || (*after_e == 'u') || (*after_e == 's')) {
        ;
      }
      --after_e;
      for (auto it = before_e; it <= after_e; ++it) curr_val_str.push_back(*it);
      for (auto it = curr_val_str.begin(); it < curr_val_str.end(); ++it) {
        if ((*it == 's') || (*it == 'u')) *it = '-';
      }
      curr_val = ::atof(curr_val_str.c_str());
      curr_val_str = std::to_string(curr_val);
      infix.replace(before_e, after_e + 1, curr_val_str);
    }
  }
}

void s21::Model::InPolish(std::string infix_with_, std::string &pofics,
                          bool *infics_error) {
  CalcNode *stack = NULL;
  int i = 0, pofix_pointer = 0, is_digit = 0;
  char pop_char = 0;
  std::string infix = "", ariphmetic = "Ppasmdwo\0", functions = "cCtRrTqlgu\0";
  double pop_val = 0.0;
  DeleteUnderscore(infix_with_);
  infix = infix_with_;
  UnaryChecker(infix);
  ParenthesesChecker(infics_error, infix);
  Exponenta(infix);
  i = 0;
  while ((infix[i]) && (!(*infics_error))) {
    if (((infix[i] <= '9') && (infix[i] >= '0')) || (infix[i] == '.') ||
        (infix[i] == 'e') || (infix[i] == 'x')) {
      if (!is_digit) {
        PofixAdd(pofics, &pofix_pointer, '|');
        is_digit = 1;
      }
      PofixAdd(pofics, &pofix_pointer, infix[i]);
    } else if ((infix[i] == '(') ||
               (functions.find(infix[i]) != std::string::npos)) {
      AddVSlash(pofics, &pofix_pointer, &is_digit);
      Push(&stack, infix[i], 0);
    } else if ((ariphmetic.find(infix[i]) != std::string::npos)) {
      AddVSlash(pofics, &pofix_pointer, &is_digit);
      while ((stack) && ((functions.find(infix[i]) != std::string::npos) ||
                         (AriphPrior(stack, infix[i]) == 0) ||
                         (AriphPrior(stack, infix[i]) == 1))) {
        Pop(&stack, &pop_char, &pop_val);
        PofixAdd(pofics, &pofix_pointer, pop_char);
      }
      Push(&stack, infix[i], 0);
    } else if (infix[i] == ')') {
      AddVSlash(pofics, &pofix_pointer, &is_digit);
      while ((stack->sym != '(') && (stack->next != NULL)) {
        Pop(&stack, &pop_char, &pop_val);
        PofixAdd(pofics, &pofix_pointer, pop_char);
      }
      if ((stack->sym != '(') && (stack->next == NULL)) {
        *infics_error = 1;
      }
      if (stack->next) Pop(&stack, &pop_char, &pop_val);
    }
    ++i;
  }
  ++i;
  ParenthesesAdder(infics_error, stack, pofics, &pofix_pointer, infix,
                   &is_digit, &i, &pop_char, &pop_val);
}

void s21::Model::DoublePop(CalcNode **stack, double *a, double *b,
                           char *curr_char, bool *pofix_error) {
  if (*stack) {
    Pop(stack, curr_char, a);
    if (*stack) {
      Pop(stack, curr_char, b);
    } else {
      *pofix_error = 1;
    }
  } else {
    *pofix_error = 1;
  }
}

void s21::Model::SinglePop(CalcNode **stack, double *a, char *curr_char,
                           bool *pofix_error) {
  if (*stack) {
    Pop(stack, curr_char, a);
  } else {
    *pofix_error = 1;
  }
}

double s21::Model::AddCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0, b = 0.0;
  char curr_char = 0;
  DoublePop(stack, &a, &b, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = a + b;
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::SubCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0, b = 0.0;
  char curr_char = 0;
  DoublePop(stack, &a, &b, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = b - a;
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::MultCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0, b = 0.0;
  char curr_char = 0;
  DoublePop(stack, &a, &b, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = b * a;
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::DivCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0, b = 0.0;
  char curr_char = 0;
  DoublePop(stack, &a, &b, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = b / a;
    Push(stack, '\0', res);
  }
  return res;
  ;
}

double s21::Model::PowCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0, b = 0.0;
  char curr_char = 0;
  DoublePop(stack, &a, &b, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = pow(b, a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::ModCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0, b = 0.0;
  char curr_char = 0;
  DoublePop(stack, &a, &b, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = fmod(b, a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::UMCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = -1 * a;
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::CosCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = cos(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::SinCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = sin(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::TanCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = tan(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::AcosCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = acos(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::AsinCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = asin(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::AtanCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = atan(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::SqrtCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = sqrt(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::LogCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = log10(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::LnCalc(CalcNode **stack, bool *pofix_error) {
  double res = 0.0, a = 0.0;
  char curr_char = 0;
  SinglePop(stack, &a, &curr_char, pofix_error);
  if (!(*pofix_error)) {
    res = log(a);
    Push(stack, '\0', res);
  }
  return res;
}

double s21::Model::StepCalc(CalcNode **stack, char oper, bool *pofix_error) {
  double res = 0.0;
  if (oper == 'a') {
    res = AddCalc(stack, pofix_error);
  } else if (oper == 's') {
    res = SubCalc(stack, pofix_error);
  } else if (oper == 'm') {
    res = MultCalc(stack, pofix_error);
  } else if (oper == 'd') {
    res = DivCalc(stack, pofix_error);
  } else if (oper == 'w') {
    res = PowCalc(stack, pofix_error);
  } else if (oper == 'o') {
    res = ModCalc(stack, pofix_error);
  } else if (oper == 'u') {
    res = UMCalc(stack, pofix_error);
  } else if (oper == 'c') {
    res = CosCalc(stack, pofix_error);
  } else if (oper == 'C') {
    res = SinCalc(stack, pofix_error);
  } else if (oper == 't') {
    res = TanCalc(stack, pofix_error);
  } else if (oper == 'R') {
    res = AcosCalc(stack, pofix_error);
  } else if (oper == 'r') {
    res = AsinCalc(stack, pofix_error);
  } else if (oper == 'T') {
    res = AtanCalc(stack, pofix_error);
  } else if (oper == 'q') {
    res = SqrtCalc(stack, pofix_error);
  } else if (oper == 'l') {
    res = LnCalc(stack, pofix_error);
  } else if (oper == 'g') {
    res = LogCalc(stack, pofix_error);
  }
  return res;
}

double s21::Model::FromPolish(const std::string &pofics, double in_double,
                              bool *pofix_error) {
  double res = 0.0, curr_double = 0;
  int double_string_pointer = 0, is_num = 1;
  size_t i = 0;
  CalcNode *stack = NULL;
  std::string double_string;
  bool slash_met = false;
  while ((i < pofics.size()) && (!(*pofix_error))) {
    if (((pofics[i] <= '9') && (pofics[i] >= '0')) || (pofics[i] == '.') ||
        (pofics[i] == 'e') || (pofics[i] == 'x') || (pofics[i] == '|')) {
      if (pofics[i] == 'x') {
        Push(&stack, '\0', in_double);
        is_num = 0;
      } else {
        if (pofics[i] != '|') {
          while ((pofics[i]) && (pofics[i] != '|')) {
            double_string += pofics[i];
            ++double_string_pointer;
            ++i;
            is_num = 1;
          }
          if (pofics[i] == '|') {
            --i;
          }
        } else {
          slash_met = true;
          if (is_num) {
            try {
              curr_double = std::stod(double_string);
            } catch (std::out_of_range const &) {
              *pofix_error = true;
            } catch (std::invalid_argument const &) {
              *pofix_error = true;
            }
            Push(&stack, '\0', curr_double);
            double_string.clear();
            double_string_pointer = 0;
          }
        }
        if (!slash_met) {
          try {
            res = std::stod(double_string);
          } catch (std::out_of_range const &) {
            *pofix_error = true;
          } catch (std::invalid_argument const &) {
            *pofix_error = true;
          }
        }
      }
    } else {
      res = StepCalc(&stack, pofics[i], pofix_error);
      is_num = 0;
    }
    ++i;
  }
  if (stack) {
    res = stack->val;
    FreeList(stack);
  }
  return res;
}

void s21::Model::Compute(std::string &infix, double in_double, bool *error) {
  std::string pofics;
  for (int i = 0; infix[i]; ++i) {
    if (infix[i] == 'i') {
      infix[i] = '.';
    } else if (infix[i] == 'P') {
      infix[i] = '(';
    } else if (infix[i] == 'p') {
      infix[i] = ')';
    }
  }
  InPolish(infix, pofics, error);
  if (!(*error)) {
    data_ = FromPolish(pofics, in_double, error);
  }
}

void s21::Model::Plot(std::string input_expression,
                      std::vector<double> &std_x_plot,
                      std::vector<double> &std_y_plot, double xmin, double xmax,
                      double xstep, bool &error) {
  double x_ax = xmin;
  for (int i = 0; input_expression[i]; ++i) {
    if (input_expression[i] == 'i') {
      input_expression[i] = '.';
    } else if (input_expression[i] == 'P') {
      input_expression[i] = '(';
    } else if (input_expression[i] == 'p') {
      input_expression[i] = ')';
    }
  }
  if (!error) {
    while ((x_ax <= xmax) && (!error)) {
      std_x_plot.push_back(x_ax);
      Compute(input_expression, x_ax, &error);
      std_y_plot.push_back(data_);
      x_ax += xstep;
    }
  } else {
    error = 1;
  }
}

double s21::Model::GetData() const { return data_; }

// Credit
double s21::Model::HundredRound(double val) {
  double res = val * 100;
  res = floor(res);
  return (res / 100);
}

void s21::Model::AnnuityLoan(double loan_amount, double interest_rate,
                             int loan_term_in_months, double &monthly_payment,
                             double &common_payment, double &overpayment) {
  double monthly_interest_rate = interest_rate / 1200;
  monthly_payment = (loan_amount * monthly_interest_rate) /
                    (1 - pow(1 + monthly_interest_rate, -loan_term_in_months));
  common_payment = monthly_payment * loan_term_in_months;
  overpayment = monthly_payment * loan_term_in_months - loan_amount;
  monthly_payment = HundredRound(monthly_payment);
  common_payment = HundredRound(common_payment);
  overpayment = HundredRound(overpayment);
}

void s21::Model::DifferentialLoan(double loan_amount, double interest_rate,
                                  int loan_term_in_months,
                                  std::string &debt_interest,
                                  std::string &monthly_payment,
                                  std::string &interest_charges) {
  double monthly_pay = loan_amount / loan_term_in_months;
  double monthly_interest_rate = interest_rate / 1200;
  double First_month = monthly_pay + loan_amount * monthly_interest_rate;
  double last_month =
      monthly_pay + (loan_amount - monthly_pay * (loan_term_in_months - 1)) *
                        monthly_interest_rate;
  double result = 0;

  int months = loan_term_in_months;
  for (int i = 0; i < months; i++) {
    result +=
        monthly_pay + (loan_amount - monthly_pay * i) * monthly_interest_rate;
  }
  char dept_interest_char[42] = "\0";
  char monthly_payment_char[42] = "\0";
  char interest_charges_char[42] = "\0";
  sprintf(dept_interest_char, "%.2lf", result);
  sprintf(monthly_payment_char, "%.2lf...%.2lf", First_month, last_month);
  result -= loan_amount;
  sprintf(interest_charges_char, "%.2lf", result);
  debt_interest = dept_interest_char;
  monthly_payment = monthly_payment_char;
  interest_charges = interest_charges_char;
}

// Deposit
void s21::Model::MoneyTransactions(std::vector<unsigned> &add_cash_days_std,
                                   std::vector<unsigned> &sub_cash_days_std,
                                   std::vector<double> &add_cash_values_std,
                                   std::vector<double> &sub_cash_values_std,
                                   unsigned current_day, double &dep_sum) {
  if ((add_cash_days_std.size() != 0) &&
      (current_day == add_cash_days_std.front())) {
    dep_sum += add_cash_values_std.front();
    add_cash_days_std.erase(add_cash_days_std.begin());
    add_cash_values_std.erase(add_cash_values_std.begin());
  }
  if ((sub_cash_days_std.size() != 0) &&
      (current_day == sub_cash_days_std.front())) {
    dep_sum -= sub_cash_values_std.front();
    sub_cash_days_std.erase(sub_cash_days_std.begin());
    sub_cash_values_std.erase(sub_cash_values_std.begin());
  }
}

void s21::Model::DepCalc(unsigned start_day, unsigned end_day,
                         std::vector<unsigned> add_cash_days_std,
                         std::vector<unsigned> sub_cash_days_std,
                         std::vector<double> add_cash_values_std,
                         std::vector<double> sub_cash_values_std,
                         double dep_proc, double nalog_proc, double dep_sum,
                         bool no_capitalization, double &profit, double &nalog,
                         double &result) {
  profit = 0;
  nalog = 0;
  result = 0;
  unsigned current_day = start_day;
  if (no_capitalization) {
    while (current_day < end_day) {
      MoneyTransactions(add_cash_days_std, sub_cash_days_std,
                        add_cash_values_std, sub_cash_values_std, current_day,
                        dep_sum);
      profit += dep_sum * dep_proc / 365 / 100;
      ++current_day;
    }
  } else {
    double current_profit;
    while (current_day < end_day) {
      MoneyTransactions(add_cash_days_std, sub_cash_days_std,
                        add_cash_values_std, sub_cash_values_std, current_day,
                        dep_sum);
      current_profit = dep_sum * dep_proc / 365 / 100;
      profit += current_profit;
      dep_sum += current_profit;
      ++current_day;
    }
  }
  nalog = profit * nalog_proc / 100;
  profit -= nalog;
  nalog = HundredRound(nalog);
  result = dep_sum + profit;
  profit = HundredRound(profit);
  result = HundredRound(result);
}
