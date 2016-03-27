CREATE SEQUENCE visit_id_seq;

DROP TABLE visit;

CREATE TABLE visit
(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('visit_id_seq'),
	"client_service_id" INTEGER NOT NULL,
	"visit_data" timestamp NOT NULL,
	"info" TEXT NOT NULL
);

ALTER TABLE visit
	ADD CONSTRAINT client_service_id_fk 
		FOREIGN KEY (client_service_id) REFERENCES client_service (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;