class MoneyManager {

public:
   MoneyManager(std::string in_instrument, double in_max_balance, int in_percentage) : 
      max_balance(in_max_balance) {
         if (in_instrument == "SP500") {
            if (in_percentage == 2) {
               volume = max_balance/1000;
               amount_to_increase = 1000;
               volume_to_increase = 1;
            } else {
               volume = 0;
               amount_to_increase = 0;
               volume_to_increase = 0;
            }
         } else if (in_instrument == "EURUSD") {
            if (in_percentage == 2) {
               volume = max_balance/10000.0;
               amount_to_increase = 100;
               volume_to_increase = 0.01;
            } else {
               volume = 0;
               amount_to_increase = 0;
               volume_to_increase = 0;
            }
         } else if (in_instrument == "GBPJPY") {
            if (in_percentage == 2) {
               volume = max_balance/10000.0;
               amount_to_increase = 100;
               volume_to_increase = 0.01;
            } else {
               volume = 0;
               amount_to_increase = 0;
               volume_to_increase = 0;
            }
         } else {
            volume = 0;
            amount_to_increase = 0;
            volume_to_increase = 0;
         }
      }
      
   void modify_volume_based_on_max_balance() {
      double current_balance = get_current_balance();
      while(current_balance >= get_max_balance() + amount_to_increase) {
         increase_max_balance(amount_to_increase);
         increase_volume(volume_to_increase);
      }
   }
   
   double get_current_balance() {
      return 3000;
   }
   
   double get_max_balance() {
      return max_balance;
   }
   
   void set_max_balance(double max_balance_) {
      max_balance = max_balance_;
   }
   
   void increase_max_balance(double value) {
      max_balance += value;
   }
   
   double get_volume() {
      return volume;
   }
   
   void set_volume(double volume_) {
      volume = volume_;
   }
   
   void increase_volume(double value) {
      volume += value;
   }
   
private:
   double max_balance;
   double volume;
   double amount_to_increase;
   double volume_to_increase;
};