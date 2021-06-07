
// class Parser {
// public:
//   Parser(int price_multiplier) : price_multiplier_(price_multiplier) {}

//   std::vector<BookSnapshot> parse(const std::string &snap_path,
//                                   const std::string &update_path) {
//     open_file_check(snap_path, update_path);

//     ob_ = read_book();
//     std::vector<BookSnapshot> results;

// #ifdef DEBUG_FLAG
//     ob_.debug_print();
// #endif

//     char symbol[10], side, update_type[10];
//     int64_t timestamp, first_update_id, last_update_id, pu;
//     double price, qty;

//     while (fscanf(input_update_, ROW_FMT.c_str(), symbol, &timestamp,
//                   &first_update_id, &last_update_id, &side, update_type,
//                   &price, &qty, &pu) > 0) {
//       if (last_update_id < ob_.last_update_id)
//         continue;

//       auto price_mul = std::llround(price * price_multiplier_);
//       if (ob_.last_update_id < last_update_id) {
//         results.push_back(dump());

// #ifdef DEBUG_FLAG
//         // printf("first %ld %ld\n", ob_.last_update_id, last_update_id);
//         // break;
//         if (results.size() % 10000 == 0) {
//           printf("num snaps: %lu\n", results.size());
//           ob_.debug_print();
//           // break;
//         } // debug purpose;
// #endif

//         ob_.timestamp = timestamp;
//         ob_.last_update_id = last_update_id;
//       }

//       if (side == 'a') {
//         if (qty == 0)
//           ob_.ask.erase(price_mul);
//         else
//           ob_.ask[price_mul] = qty;
//       }

//       if (side == 'b') {
//         if (qty == 0)
//           ob_.bid.erase(price_mul);
//         else
//           ob_.bid[price_mul] = qty;
//       }
//     }

//     return results;
//   }

// private:
//   int price_multiplier_;
//   OrderBook ob_;
//   FILE *input_snap_, *input_update_;

//   BookSnapshot dump() {
//     BookSnapshot s;
//     s.timestamp = ob_.timestamp;
//     s.last_update_id = ob_.last_update_id;
//     auto ask_iter = ob_.ask.begin();
//     auto bid_iter = ob_.bid.begin();
//     for (size_t i = 0; i < LEVELS; i++, ++ask_iter, ++bid_iter) {
//       s.ask_prices[i] = ask_iter->first / (double)price_multiplier_;
//       s.ask_sizes[i] = ask_iter->second;
//       s.bid_prices[i] = bid_iter->first / (double)price_multiplier_;
//       s.bid_sizes[i] = bid_iter->second;
//     }
//     return s;
//   }

//   void open_file_check(const std::string &snap_path,
//                        const std::string &update_path) {
//     input_snap_ = fopen(snap_path.c_str(), "r");
//     input_update_ = fopen(update_path.c_str(), "r");
//     auto buf = new char[1000];

//     fscanf(input_snap_, "%s", buf);
//     if (std::string(buf) != HEADER)
//       throw std::runtime_error("Snapshot header not match");

//     fscanf(input_update_, "%s", buf);
//     if (std::string(buf) != HEADER)
//       throw std::runtime_error("Update header not match");

//     delete buf;
//   }

//   void close_files() {
//     fclose(input_snap_);
//     fclose(input_update_);
//   }
// };
