#!/usr/bin/env bash
# Common functions for EA rules.

## MATH ##

# Get the max of two values.
min() {
  echo $(( $1 < ${2:-0} ? $1 : ${2:-0} ))
}

# Get the max of two values.
max() {
  echo $(( $1 > ${2:-0} ? $1 : ${2:-0} ))
}

## OPTIMIZATION ##

# Enables/disable all optimization settings.
ea_optimize_all_set() {
  input_set .*,F $1
}

# Optimize specific parameter.
ea_set_optimize_param() {
  local param=$1
  local start=$2
  local end=$3
  local step=${4:-1}
  # Optimization settings (F-On, 1-Min, 2-Step, 3-Max).
  input_set ^$param,F 1
  input_set ^$param,1 $start
  input_set ^$param,2 $step
  input_set ^$param,3 $end
}

# Close all orders at the given hour.
ea_orders_close_all_at() {
  local hour=${1:-22}
  ea_actions_enabled 1
  ea_actions_reset_all
  ea_set_action 1 15 10
  input_set ^MarketSpecificHour hour
}

# Sets default params for optimization.
ea_default_sets() {
  # Disable all optimization settings.
  ea_optimize_all_set 0
  # Close orders after some time.
  input_set ^CloseOrderAfterXHours 48
  # Disable account conditions.
  ea_actions_enabled 0
  # Disable boosting.
  ea_boost_enabled 0
  # Do not set profit factor trade limits.
  input_set ^ProfitFactorMinToTrade 0
  input_set ^ProfitFactorMaxToTrade 0
  # Set default lot size.
  ea_set_lotsize 0.01
}

## TRADE ##

ea_set_lotsize() {
  input_set ^LotSize $1
}

## ACTIONS ##

# Enables/disables account conditional actions.
ea_actions_enabled() {
  input_set Account_Conditions_Active $1
}

# Reset/set all actions.
ea_actions_reset_all() {
  input_set ^Account_Condition_[0-9]\\+   ${1:-0}
  input_set ^Market_Condition_[0-9]\\+    ${2:-0}
  input_set ^Action_On_Condition_[0-9]\\+ ${3:-0}
}

# Set action with the specific condition.
ea_set_action() {
  input_set ^Account_Condition_1   $1
  input_set ^Market_Condition_1    $2
  input_set ^Action_On_Condition_1 $3
}

## BOOSTING ##

# Enables/disables boosting.
ea_boost_enabled() {
  input_set ^Boosting_Enabled $1
}

## STRATEGIES ##

# Enables/disables strategies.
ea_strategies_enabled() {
  input_set "${2}.*_Active" ${1:-1}
}

# Set strategy parameters.
ea_set_strategy_param() {
  local strategy=$1
  local pattern=$2
  local value=$3
  input_set ^${strategy}.*_${pattern} $3
}

# Enables/disables strategy.
ea_strategy_enabled() {
  input_set ^${1}_Active ${2:-1}
}

# Sets default params to test core logic.
ea_strategy_default_sets() {
  local strategy=$1
  # Set default trade params.
  ea_default_sets
  # Disable all active strategies.
  ea_strategies_enabled 0
  # Enables single strategy.
  input_set ^${strategy}_Active 1
  # Reset signal methods.
  input_set ^${strategy}.*_SignalMethod 0
}

## TRAILINGS ##

# Set neutral trail method.
ea_set_neutral_trail() {
  local strategy=${1}
  local method=${2:-0}
  input_set ^${strategy}_TrailingProfitMethod ${method}
  input_set ^${strategy}_TrailingStopMethod ${method}
  input_set ^MinPipGap 10
}

## GETTERS ##

ea_get_period() {
  ini_get ^TestPeriod
}

## MISC ##
