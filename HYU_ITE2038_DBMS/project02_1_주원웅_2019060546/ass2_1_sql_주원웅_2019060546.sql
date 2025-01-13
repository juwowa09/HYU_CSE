DROP DATABASE IF EXISTS DB_2019060546;
CREATE DATABASE DB_2019060546;
USE DB_2019060546;

CREATE TABLE users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE stock_accounts (
    account_id INT PRIMARY KEY,
    user_id INT NOT NULL,
    cash_balance INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE stocks (
    stock_id INT PRIMARY KEY,
    stock_name VARCHAR(255) NOT NULL,
    current_price INT NOT NULL
);

CREATE TABLE holdings (
    account_id INT NOT NULL,
    stock_id INT NOT NULL,
    purchase_price INT NOT NULL COMMENT '매수 가격',
    cur_price INT NOT NULL COMMENT '최근 거래 가격',
    quantity INT NOT NULL COMMENT '보유 주식 수량',
    available_quantity INT NOT NULL COMMENT '매도 가능 주식 수량',
    profit_or_loss DECIMAL(15, 2) COMMENT '평가 손익',
    updown DECIMAL(15, 2) COMMENT '등락률',
    PRIMARY KEY(account_id, stock_id, purchase_price),
    FOREIGN KEY (account_id) REFERENCES stock_accounts(account_id),
    FOREIGN KEY (stock_id) REFERENCES stocks(stock_id)
);

CREATE TABLE transactions (
    account_id INT NOT NULL,
    stock_id INT NOT NULL,
    transaction_date TIMESTAMP NOT NULL,
    transaction_status ENUM('Pending', 'Completed', 'Cancelled') DEFAULT 'Pending' NOT NULL COMMENT '거래 상태',
    transaction_type ENUM('Buy', 'Sell') NOT NULL COMMENT '거래 종류',
    trade_type ENUM('Market', 'Limit') NOT NULL COMMENT '시장가, 지정가 구분',
    quantity INT NOT NULL COMMENT '거래 수량',
    price INT NOT NULL COMMENT '거래 가격',
    PRIMARY KEY (account_id, stock_id, transaction_date),
    FOREIGN KEY (account_id) REFERENCES stock_accounts(account_id),
    FOREIGN KEY (stock_id) REFERENCES stocks(stock_id)
);

CREATE TABLE order_books (
    order_id INT PRIMARY KEY AUTO_INCREMENT,
    stock_id INT NOT NULL,
    order_type ENUM('Buy', 'Sell') NOT NULL,
    order_price INT NOT NULL,
    order_quantity INT NOT NULL,
    datetime TIMESTAMP NOT NULL,
    FOREIGN KEY (stock_id) REFERENCES stocks(stock_id)
);

CREATE TABLE stock_statistics (
    stock_id INT NOT NULL,
    stat_type ENUM('PriceIncrease', 'VolumeRanking') NOT NULL,
    period VARCHAR(255) NOT NULL,
    stat_value DECIMAL(15, 2) NOT NULL,
    PRIMARY KEY (stock_id, stat_type, period),
    FOREIGN KEY (stock_id) REFERENCES stocks(stock_id)
);