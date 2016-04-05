CREATE SEQUENCE vid_id_seq;

DROP TABLE vid;

CREATE TABLE vid(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('vid_id_seq'),
	"name" TEXT NOT NULL
);

CREATE SEQUENCE vid_service_id_seq;
DROP TABLE vid_service;
CREATE TABLE vid_service(
	"id" INTEGER PRIMARY KEY NOT NULL DEFAULT nextval('vid_service_id_seq'),
	"service_id" INTEGER NOT NULL,
	"vid_id" INTEGER NOT NULL
);

ALTER TABLE vid_service 
	ADD CONSTRAINT vid_id_fk 
		FOREIGN KEY (vid_id) REFERENCES vid (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;
			
ALTER TABLE vid_service 
	ADD CONSTRAINT service_id_fk 
		FOREIGN KEY (service_id) REFERENCES service (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;