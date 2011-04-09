CREATE TABLE exchange_rates (
    symbol character(3) NOT NULL,
    exchange_to double precision,
    exchange_from double precision,
    added_at date DEFAULT current_date NOT NULL,
    PRIMARY KEY (symbol, added_at)
);
