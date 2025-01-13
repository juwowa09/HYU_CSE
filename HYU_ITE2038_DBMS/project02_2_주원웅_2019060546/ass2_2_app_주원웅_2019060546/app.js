const express = require('express'); // express 변수에 초기화
const app = express();              // app 을 통해서 express 의 작업 수행

const mysql = require('mysql2');
const db_info = {
    host: "localhost",
    port: "3306",
    user: "root",
    password: "wn06055656",
    database: "DB_2019060546",
}

const sql_connection = mysql.createConnection(db_info);
sql_connection.connect();

let i = 1;
const initStock = "SELECT * FROM stocks";
sql_connection.query(initStock, (error, results, fields) => {
    if (error) throw error;
    if (results == 0) {
        let i = 1;
        sql_connection.query("insert into stocks (stock_id, stock_name, current_price) values(?,'삼성전자',58700)", [i++], (error, results1, fields) => { if (error) throw error; });

        sql_connection.query("insert into stocks (stock_id, stock_name, current_price) values(?,'SK하이닉스',194000)", [i++], (error, results1, fields) => { if (error) throw error; });

        sql_connection.query("insert into stocks (stock_id, stock_name, current_price) values(?,'카카오',37350)", [i++], (error, results1, fields) => { if (error) throw error; });

        sql_connection.query("insert into stocks (stock_id, stock_name, current_price) values(?,'네이버',175700)", [i++], (error, results1, fields) => { if (error) throw error; });

        sql_connection.query("insert into stocks (stock_id, stock_name, current_price) values(?,'현대차',216500)", [i++], (error, results1, fields) => { if (error) throw error; });
    }
});

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.set("view engine", "ejs");
app.set("views", "./views");

app.get("/", (req, res) => {    // req 는 요청, res 는 요청에 대한 응답.
    res.render("home");     // call_back 함수를 통해서 요청이 처리되었음을 응답을 통해 보낸다.
}) // root page(url = / 형태) 로 들어온 경우 필요한 작업을 명시 

app.get("/login", (req, res) => {    // req 는 요청, res 는 요청에 대한 응답.
    res.render("loginPage");     // call_back 함수를 통해서 요청이 처리되었음을 응답을 통해 보낸다.
})


app.post("/login", (req, res) => {
    const { userID, userPassword } = req.body;

    // 로그인 확인
    sql_connection.query(
        "SELECT id FROM users WHERE email = ? AND password = ?",
        [userID, userPassword],
        (error, results) => {
            if (error) throw error;

            if (results.length > 0) {
                const userId = results[0].id;

                // 로그인 성공 후 해당 사용자의 계좌 목록을 조회
                sql_connection.query(
                    "SELECT account_id, cash_balance FROM stock_accounts WHERE user_id = ?",
                    [userId],
                    (accError, accResults) => {
                        if (accError) throw accError;

                        // 계좌 목록을 'accounts'로 전달
                        res.render("dashboard", { userID, id: userId, accounts: accResults });
                    }
                );
            } else {
                res.render("loginPage", { loginResult: 'fail' });
            }
        }
    );
});

function generateAccountNumber() {
    const prefix = "123";  // 3자리 접두사
    const randomNumber = Math.floor(100000 + Math.random() * 900000);  // 6자리 랜덤 숫자 생성
    return `${prefix}${randomNumber}`;  // 접두사 + 6자리 랜덤 숫자 조합 (총 9자리)
}

app.post("/dashboard", (req, res) => {
    const id = req.body.id;
    const userID = req.body.userID;
    const action = req.body.action;

    if (action === "createAccount") {
        const accountNumber = generateAccountNumber();

        // 계좌 생성 쿼리 실행
        sql_connection.query(
            "INSERT INTO stock_accounts (account_id, user_id, cash_balance) VALUES (?, ?, ?)",
            [accountNumber, id, 0],
            (error, results) => {
                if (error) throw error;

                // 계좌 생성 후 최신 계좌 목록 조회
                sql_connection.query(
                    "SELECT account_id, cash_balance FROM stock_accounts WHERE user_id = ?",
                    [id],
                    (accError, accResults) => {
                        if (accError) throw accError;

                        // 최신 계좌 목록과 함께 대시보드 렌더링
                        res.render("dashboard", { userID, id, accounts: accResults });
                    }
                );
            }
        );
    }
    else if (action == "deposit") {
        const account_id = req.body.account_id;
        const balance = req.body.amount;

        sql_connection.query("UPDATE stock_accounts SET cash_balance = cash_balance + ? WHERE account_id = ?", [balance, account_id], (error, results, fields) => {
            if (error) throw error;
            else {
                sql_connection.query(
                    "SELECT account_id, cash_balance FROM stock_accounts WHERE user_id = ?",
                    [id],
                    (accError, accResults) => {
                        if (accError) throw accError;

                        // 최신 계좌 목록과 함께 대시보드 렌더링
                        res.render("dashboard", { userID, id, accounts: accResults });
                    }
                );
            }
        })
    } else if (action == "datail") {
        const account_id = req.body.account_id;
        // 1. 계좌의 cash_balance를 조회
        const accountQuery = "SELECT cash_balance FROM stock_accounts WHERE account_id = ?";
        sql_connection.query(accountQuery, [account_id], (error, results, fields) => {
            if (error) throw error;
            const cashBalance = results[0].cash_balance;

            // 2. 보유 중인 주식 목록을 조회
            const holdingsQuery = `
            SELECT stocks.stock_name, holdings.quantity, 
                   (stocks.current_price * holdings.quantity) AS total_value
            FROM holdings
            JOIN stocks ON holdings.stock_id = stocks.stock_id
            WHERE holdings.account_id = ?`;
            sql_connection.query(holdingsQuery, [account_id], (error, holdings, fields) => {
                if (error) throw error;
                else {
                    const stocksQuery = `
            SELECT * FROM stocks`;
                    sql_connection.query(stocksQuery, (error, stocks, fields) => {
                        if (error) throw error;
                        else {
                            res.render("account", { cashBalance, holdings: holdings, stocks: stocks, userID, id, account_id });
                        }
                    });
                }
                // 계좌 정보와 주식 목록을 템플릿으로 전달
            });
        });
    }
})

app.post("/account", (req, res) => {
    const id = req.body.id;
    const userID = req.body.userID;
    const action = req.body.action;

    if (action == "returnToDashboard") {
        sql_connection.query(
            "SELECT account_id, cash_balance FROM stock_accounts WHERE user_id = ?",
            [id],
            (accError, accResults) => {
                if (accError) throw accError;

                // 계좌 목록을 'accounts'로 전달
                res.render("dashboard", { userID, id, accounts: accResults });
            }
        );
    }
})

app.post("/transaction", (req, res) => {
    const action = req.body.action;
    const stock_name = req.body.stock_name;
    const stock_id = req.body.stock_id;
    const account_id = req.body.account_id;
    const buyquery = "SELECT stock_id, price, SUM(quantity) AS total_quantity, transaction_type, transaction_status FROM transactions GROUP BY stock_id, price, transaction_type, transaction_status having transaction_type = 'Buy' and transaction_status = 'Pending' and stock_id = ? order by price desc limit 5"
    const sellquery = "SELECT stock_id, price, SUM(quantity) AS total_quantity, transaction_type, transaction_status FROM transactions GROUP BY stock_id, price, transaction_type, transaction_status having transaction_type = 'Sell' and transaction_status = 'Pending' and stock_id = ? order by price asc limit 5";
    const cash_balance = req.body.cash_balance;

    if (action == "transaction") {
        sql_connection.query("select cash_balance from stock_accounts where account_id = ?", [account_id], (error, cashResults, fields) => {
            if (error) throw error;
            sql_connection.query(buyquery, [stock_id], (error, buyResults, fields) => {

                if (error) throw error;
                else {
                    sql_connection.query(sellquery, [stock_id], (error, sellResults, fields) => {
                        res.render("transaction", { buyResults: buyResults, sellResults: sellResults, cash_balance: cashResults[0].cash_balance, account_id, stock_name, stock_id });
                    })
                }
            })
        })
    } else if (action == "buy") {
        const buy_price = req.body.buy_price;
        const buy_quantity = req.body.buy_quantity;

        if (cash_balance < buy_price * buy_quantity) {

            sql_connection.query(buyquery, [stock_id], (error, buyResults, fields) => {
                if (error) throw error;
                else {
                    sql_connection.query(sellquery, [stock_id], (error, sellResults, fields) => {
                        res.render("transaction", { buyResults: buyResults, sellResults: sellResults, cash_balance, account_id, stock_name, stock_id, tResult: 'fail' });
                    })
                }
            })
        } else {    // 구매 구현
            const buy_req_query = "select * from transactions where transaction_status = ? and transaction_type = ? order by price asc limit 1;";
            sql_connection.query(buy_req_query, ['Pending', 'Sell'], (error, searchResults, fields) => {
                if (error) throw error;
                if (searchResults.length > 0) {
                    const currentDateTime = new Date().toISOString().slice(0, 19).replace('T', ' ');

                    if (searchResults[0].price > buy_price) {
                        sql_connection.query("insert into transactions (account_id,stock_id,transaction_date,transaction_status,transaction_type,trade_type,quantity,price) values(?,?,?,?,?,?,?,?)", [account_id, stock_id, currentDateTime, 'Pending', 'Buy', 'Limit', buy_quantity, buy_price], (error, insertResults, fields) => {
                            if (error) throw error;
                        })
                        sql_connection.query("update stock_accounts set cash_balance = cash_balance - ? where account_id = ?", [buy_price * buy_quantity, account_id], (error, results, fields) => {
                            if (error) throw error;
                        })
                    } else {
                        if (searchResults[0].quantity > buy_quantity)   // 수량 업데이트, 계좌 가격
                        {
                            const sell_account = searchResults[0].account_id;
                            const time = searchResults[0].transaction_date;
                            const remain = searchResults[0].quantity - buy_quantity;

                            // 가지고있던 사람 수정
                            sql_connection.query("update transactions set quantity = ? where account_id = ? and stock_id = ? and transaction_date = ?", [remain, sell_account, stock_id, time], (error, updateResults, fields) => {
                                if (error) throw error;
                                sql_connection.query("update stock_accounts set cash_balance = cash_balance + ? where account_id = ?", [buy_price * buy_quantity, sell_account], (error, results, fields) => {
                                    if (error) throw error;
                                })
                                sql_connection.query("select quantity from holdings where account_id = ? and stock_id = ?", [sell_account, stock_id], (error, holdingsResults, fields) => {
                                    if (error) throw error;
                                    if (holdingsResults > 0) {
                                        if (holdingsResults[0].quantity == buy_quantity) {
                                            sql_connection.query("delete from holdings where account_id = ? and stock_id = ?", [sell_account, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        } else {
                                            sql_connection.query("update holdings set quantity = quantity - ? where account_id = ? and stock_id = ?", [buy_quantity, sell_account, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        }
                                    }
                                })
                            });
                            // 새롭게 산 사람 수정
                            sql_connection.query("insert into transactions (account_id,stock_id,transaction_date,transaction_status,transaction_type,trade_type,quantity,price) values(?,?,?,?,?,?,?,?)", [account_id, stock_id, currentDateTime, 'Completed', 'Buy', 'Market', buy_quantity, searchResults[0].price], (error, insertResults, fields) => {
                                if (error) throw error;
                                sql_connection.query("update stock_accounts set cash_balance = cash_balance - ? where account_id = ?", [buy_price * buy_quantity, account_id], (error, results, fields) => {
                                    if (error) throw error;
                                })
                                sql_connection.query("select * from holdings where stock_id = ? and account_id = ?", [stock_id, account_id], (error, holdingsResults, fields) => {
                                    if (error) throw error;
                                    if (holdingsResults == 0) {
                                        sql_connection.query("insert into holdings (account_id, stock_id, purchase_price, cur_price, quantity, available_quantity) values(?,?,?,?,?,?)", [account_id, stock_id, searchResults[0].price, searchResults[0].price, buy_quantity, buy_quantity], (error, results, fields) => {
                                            if (error) throw error;
                                        })
                                    } else {
                                        sql_connection.query("update holdings set quantity = quantity + ? , available_quantity = available_quantity + ? where account_id = ? and stock_id = ?", [buy_quantity, buy_quantity, account_id, stock_id], (error, results, fields) => {
                                            if (error) throw error;
                                        })
                                    }
                                })
                            })
                        } else if (searchResults[0].quantity == buy_quantity) { // 구매량 동일한경우
                            const sell_account = searchResults[0].account_id;
                            const time = searchResults[0].transaction_date;
                            const remain = searchResults[0].quantity - buy_quantity;
                            //가지고있던 사람 수정
                            sql_connection.query("update transactions set quantity = ?, transaction_status = ? where account_id = ? and stock_id = ? and transaction_date = ?", [remain, 'Completed', sell_account, stock_id, time], (error, updateResults, fields) => {
                                if (error) throw error;
                                sql_connection.query("update stock_accounts set cash_balance = cash_balance + ? where account_id = ?", [buy_price * buy_quantity, sell_account], (error, results, fields) => {
                                    if (error) throw error;
                                })
                                sql_connection.query("select quantity from holdings where account_id = ? and stock_id = ?", [sell_account, stock_id], (error, holdingsResults, fields) => {
                                    if (error) throw error;
                                    if (holdingsResults > 0) {
                                        if (holdingsResults[0].quantity == buy_quantity) {
                                            sql_connection.query("delete from holdings where account_id = ? and stock_id = ?", [sell_account, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        } else {
                                            sql_connection.query("update holdings set quantity = quantity + ? , available_quantity = available_quantity + ? , purchase_price = ? where account_id = ? and stock_id = ?", [buy_quantity, buy_quantity, searchResults[0].price, account_id, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        }
                                    }
                                })
                            });
                            // 새롭게 산사람 수정
                            sql_connection.query("insert into transactions (account_id,stock_id,transaction_date,transaction_status,transaction_type,trade_type,quantity,price) values(?,?,?,?,?,?,?,?)", [account_id, stock_id, currentDateTime, 'Completed', 'Buy', 'Market', buy_quantity, searchResults[0].price], (error, insertResults, fields) => {
                                if (error) throw error;
                                sql_connection.query("update stock_accounts set cash_balance = cash_balance - ? where account_id = ?", [buy_price * buy_quantity, account_id], (error, results, fields) => {
                                    if (error) throw error;
                                })
                                sql_connection.query("select * from holdings where stock_id = ? and account_id = ?", [stock_id, account_id], (error, holdingsResults, fields) => {
                                    if (error) throw error;
                                    if (holdingsResults == 0) {
                                        sql_connection.query("insert into holdings (account_id, stock_id, purchase_price, cur_price, quantity, available_quantity) values(?,?,?,?,?,?)", [account_id, stock_id, searchResults[0].price, searchResults[0].price, buy_quantity, buy_quantity], (error, results, fields) => {
                                            if (error) throw error;
                                        })
                                    } else {
                                        sql_connection.query("update holdings set quantity = quantity + ? , available_quantity = available_quantity + ? , purchase_price = ? where account_id = ? and stock_id = ?", [buy_quantity, buy_quantity, searchResults[0].price, account_id, stock_id], (error, results, fields) => {
                                            if (error) throw error;
                                        })
                                    }
                                })
                            })
                        }
                    }
                }
            })
        }
        sql_connection.query("select cash_balance from stock_accounts where account_id = ?", [account_id], (error, cashResults, fields) => {
            if (error) throw error;
            sql_connection.query(buyquery, [stock_id], (error, buyResults, fields) => {

                if (error) throw error;
                else {
                    sql_connection.query(sellquery, [stock_id], (error, sellResults, fields) => {
                        res.render("transaction", { buyResults: buyResults, sellResults: sellResults, cash_balance: cashResults[0].cash_balance, account_id, stock_name, stock_id });
                    })
                }
            })
        })
    } else if (action == "sell") {
        const sell_price = req.body.sell_price;
        const sell_quantity = req.body.sell_quantity;

        sql_connection.query("select quantity from holdings where account_id = ? and stock_id = ?", [account_id, stock_id], (error, results, fields) => {
            if (error) throw error;
            if (results.length > 0 && results[0].quantity < sell_quantity) {
                sql_connection.query(buyquery, [stock_id], (error, buyResults, fields) => {
                    if (error) throw error;
                    else {
                        sql_connection.query(sellquery, [stock_id], (error, sellResults, fields) => {
                            if (error) throw error;
                            res.render("transaction", { buyResults: buyResults, sellResults: sellResults, cash_balance, account_id, stock_name, stock_id, tResult: 'fail' });
                        })
                    }
                })
            } else { // 판매 구현
                const sell_req_query = "select * from transactions where transaction_status = ? and transaction_type = ? order by price desc limit 1;";
                sql_connection.query(sell_req_query, ['Pending', 'Buy'], (error, searchResults, fields) => {
                    if (error) throw error;
                    if (searchResults.length > 0) {
                        const currentDateTime = new Date().toISOString().slice(0, 19).replace('T', ' ');

                        if (searchResults[0].price < sell_price) {
                            sql_connection.query("insert into transactions (account_id,stock_id,transaction_date,transaction_status,transaction_type,trade_type,quantity,price) values(?,?,?,?,?,?,?,?)", [account_id, stock_id, currentDateTime, 'Pending', 'Sell', 'Limit', sell_quantity, sell_price], (error, insertResults, fields) => {
                                if (error) throw error;
                            })
                            sql_connection.query("update holdings set available_quantity = available_quantity - ?,quantity = quantity - ? where account_id = ? and stock_id = ?", [sell_quantity, sell_quantity, account_id, stock_id], (error, results, fields) => {
                                if (error) throw error;
                            })
                        } else {
                            if (searchResults[0].quantity > sell_quantity)   // 바로 판매, 계좌 가격
                            {
                                const buy_account = searchResults[0].account_id;
                                const time = searchResults[0].transaction_date;
                                const remain = searchResults[0].quantity - sell_quantity;

                                // 산 사람 수정
                                sql_connection.query("update transactions set quantity = ? where account_id = ? and stock_id = ? and transaction_date = ?", [remain, buy_account, stock_id, time], (error, updateResults, fields) => {
                                    if (error) throw error;

                                    sql_connection.query("select quantity from holdings where account_id = ? and stock_id = ?", [buy_account, stock_id], (error, holdingsResults, fields) => {
                                        if (error) throw error;
                                        if (holdingsResults == 0) {
                                            sql_connection.query("insert into holdings (account_id, stock_id, purchase_price, cur_price, quantity, available_quantity) values (?,?,?,?,?,?)", [buy_account, stock_id, searchResults[0].price, searchResults[0].price, sell_quantity, sell_quantity], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        } else {
                                            sql_connection.query("update holdings set quantity = quantity + ?, available_quantity = available_quantity + ?, purchase_price = ?, cur_price = ? where account_id = ? and stock_id = ?", [sell_quantity, sell_quantity, searchResults[0].price, searchResults[0].price, buy_account, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        }
                                    })
                                });
                                // 판 사람 수정
                                sql_connection.query("insert into transactions (account_id,stock_id,transaction_date,transaction_status,transaction_type,trade_type,quantity,price) values(?,?,?,?,?,?,?,?)", [account_id, stock_id, currentDateTime, 'Completed', 'Sell', 'Market', sell_quantity, searchResults[0].price], (error, insertResults, fields) => {
                                    if (error) throw error;
                                    sql_connection.query("update stock_accounts set cash_balance = cash_balance + ? where account_id = ?", [searchResults[0].price * sell_quantity, account_id], (error, results, fields) => {
                                        if (error) throw error;
                                    })
                                    sql_connection.query("select * from holdings where account_id = ? and stock_id = ?", [account_id, stock_id], (error, results, fields) => {
                                        if (error) throw error;
                                        if (results[0].quantity == sell_quantity) {
                                            sql_connection.query("delete from holdings where account_id = ? and stock_id = ?", [account_id, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        } else {
                                            sql_connection.query("update holdings set available_quantity = available_quantity - ?, quantity = quantity - ? where stock_id = ? and account_id = ?", [sell_quantity, sell_quantity, stock_id, account_id], (error, holdingsResults, fields) => {
                                                if (error) throw error;

                                            })
                                        }
                                    })
                                })
                            } else if (searchResults[0].quantity == sell_quantity) { // 구매량 동일한경우
                                const buy_account = searchResults[0].account_id;
                                const time = searchResults[0].transaction_date;
                                const remain = searchResults[0].quantity - sell_quantity;

                                // 가지고있던 사람 수정
                                sql_connection.query("update transactions set quantity = ?, transaction_status = ? where account_id = ? and stock_id = ? and transaction_date = ?", [remain, 'Completed', buy_account, stock_id, time], (error, updateResults, fields) => {
                                    if (error) throw error;

                                    sql_connection.query("select quantity from holdings where account_id = ? and stock_id = ?", [buy_account, stock_id], (error, holdingsResults, fields) => {
                                        if (error) throw error;
                                        if (holdingsResults == 0) {
                                            sql_connection.query("insert into holdings (account_id,stock_id,purchase_price,cur_price,quantity,available_quantity) values (?,?,?,?,?,?)", [buy_account, stock_id, searchResults[0].price, searchResults[0].price, sell_quantity, sell_quantity], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        } else {
                                            sql_connection.query("update holdings set quantity = quantity + ?, available_quantity = available_quantity + ?, purchase_price = ?, cur_price = ? where account_id = ? and stock_id = ?", [sell_quantity, sell_quantity, searchResults[0].price, searchResults[0].price, buy_account, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        }
                                    })
                                });
                                // 판 사람 수정
                                sql_connection.query("insert into transactions (account_id,stock_id,transaction_date,transaction_status,transaction_type,trade_type,quantity,price) values(?,?,?,?,?,?,?,?)", [account_id, stock_id, currentDateTime, 'Completed', 'Sell', 'Market', sell_quantity, searchResults[0].price], (error, insertResults, fields) => {
                                    if (error) throw error;
                                    sql_connection.query("update stock_accounts set cash_balance = cash_balance + ? where account_id = ?", [searchResults[0].price * sell_quantity, account_id], (error, results, fields) => {
                                        if (error) throw error;
                                    })
                                    sql_connection.query("select * from holdings where account_id = ? and stock_id = ?", [account_id, stock_id], (error, results, fields) => {
                                        if (error) throw error;
                                        if (results[0].quantity == sell_quantity) {
                                            sql_connection.query("delete from holdings where account_id = ? and stock_id = ?", [account_id, stock_id], (error, results, fields) => {
                                                if (error) throw error;
                                            })
                                        } else {
                                            sql_connection.query("update holdings set available_quantity = available_quantity - ?, quantity = quantity - ? where stock_id = ? and account_id = ?", [sell_quantity, sell_quantity, stock_id, account_id], (error, holdingsResults, fields) => {
                                                if (error) throw error;

                                            })
                                        }
                                    })
                                })
                            }
                        }
                    }
                })
            }
        })
        console.log("sell");
        sql_connection.query("select cash_balance from stock_accounts where account_id = ?", [account_id], (error, cashResults, fields) => {
            if (error) throw error;
            sql_connection.query(buyquery, [stock_id], (error, buyResults, fields) => {

                if (error) throw error;
                else {
                    sql_connection.query(sellquery, [stock_id], (error, sellResults, fields) => {
                        res.render("transaction", { buyResults: buyResults, sellResults: sellResults, cash_balance: cashResults[0].cash_balance, account_id, stock_name, stock_id });
                    })
                }
            })
        })
    }
})

app.get("/signup", (req, res) => {    // req 는 요청, res 는 요청에 대한 응답.
    res.render("signupPage");     // call_back 함수를 통해서 요청이 처리되었음을 응답을 통해 보낸다.
})

app.post("/signup", (req, res) => {    // req 는 요청, res 는 요청에 대한 응답.
    const { username, email, password } = req.body;
    sql_connection.query('SELECT * FROM users where email = ?', [email], (error, results, fields) => {
        if (error) throw error;
        if (results > 0) {
            res.send("이미 존재하는 아이디 입니다.");
        }
        else {
            sql_connection.query('insert into users(username, email, password) values(?,?,?)', [username, email, password], (error, result, fields) => {
                if (error) throw error;
                else {
                    res.send("회원가입 완료");
                }
            });
        }
    })
})

app.listen(3000, () => {
    console.log("서버 실행중"); // call back 으로 콘솔창에 로그를 출력함
}); // 3000 포트의 request 를 listen 하겠다.
