CREATE SEQUENCE client_service_id_seq;

DROP TABLE client_service;

CREATE TABLE client_service
(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('client_service_id_seq'::regclass),
	"client_id" INTEGER NOT NULL,
	"service_id" INTEGER NOT NULL
);

ALTER TABLE client_service 
	ADD CONSTRAINT client_id_fk 
		FOREIGN KEY (client_id) REFERENCES client (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;
ALTER TABLE client_service 
	ADD CONSTRAINT service_id_fk 
		FOREIGN KEY (service_id) REFERENCES service (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;