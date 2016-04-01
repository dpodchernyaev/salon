CREATE SEQUENCE email_history_id_seq;

DROP TABLE email_history;

CREATE TABLE email_history
(
	"id" integer PRIMARY KEY NOT NULL DEFAULT nextval('email_history_id_seq'),
	"client_id" integer NOT NULL,
	"email" TEXT NOT NULL,
	"date" DATE NOT NULL
);