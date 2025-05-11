# pg_iban

`pg_iban` is a PostgreSQL extension providing an `IBAN` (International Bank Account Number) data type and several utility functions for validation and manipulation of IBANs.

⚠️ **DISCLAIMER: This extension has not been tested in production. You use it at your own risk.**

---

## Features

- **Custom `iban` Data Type**: A fully functional PostgreSQL data type for storing IBANs.
- **Validation**: Functions to validate IBAN format and checksum.
- **Comparison Operators**: Custom operators for comparison (`=`, `<`, `>`, etc.) fully integrated with PostgreSQL's indexing system.
- **Index Support**: B-tree and hash indexing for efficient querying.
- **New in Version 1.1**:
  - Functions for extracting country (`iban_country`) and BBAN (`iban_bban`) parts from an IBAN.
  - Function to format IBANs into human-readable form (`iban_format`).

---

## Installation

1. Clone the repository:
```bash
git clone https://github.com/rom8726/pg_iban.git
cd pg_iban
```

2. Build and install the extension:
```bash
make
make install
```

3. Add `pg_iban` to `shared_preload_libraries` in `postgresql.conf`:
```conf
shared_preload_libraries = 'pg_iban'
```

4. Load the extension into your database:
```sql
CREATE EXTENSION pg_iban;
```

---

## Upgrading from Previous Versions

If you are upgrading from version `1.0` to `1.1`, you can apply the update using:
```sql
ALTER EXTENSION pg_iban UPDATE TO '1.1';
```
This will add the following new functions:
- `iban_country(iban) RETURNS text`: Extracts the country code from an IBAN.
- `iban_bban(iban) RETURNS text`: Extracts the BBAN (Basic Bank Account Number) from an IBAN.
- `iban_format(iban) RETURNS text`: Formats an IBAN in a more readable format.

---

## Functions

### IBAN Validation
- `iban_valid(text) RETURNS bool`: Returns `TRUE` if the provided string is a valid IBAN, `FALSE` otherwise.

### New in Version 1.1
- `iban_country(iban) RETURNS text`: Extracts the country code from an IBAN.
- `iban_bban(iban) RETURNS text`: Extracts the BBAN (Basic Bank Account Number) from an IBAN.
- `iban_format(iban) RETURNS text`: Formats an IBAN in a human-readable format.

### Operators
The extension provides custom operators for the `iban` type:
- `=` (Equality)
- `<`, `<=`, `>`, `>=` (Comparison)
- `<>` (Not Equal)

### Indexing
- B-tree and hash indexes are supported for the `iban` type.

---

## Testing

### Unit Tests
Unit tests ensure that individual functions and behaviors of the extension are implemented correctly. The extension uses `cmocka` for unit testing.

To run the unit tests:
```bash
make docker-unit-test
```

### Integration Tests
Integration tests validate the extension in a live PostgreSQL environment, ensuring end-to-end correctness.

To run the integration tests:
```bash
make docker-integration-test
```

Integration tests include functions such as:
- Creating the `iban` type in the database.
- Validating correct and incorrect IBANs.
- Using the `iban` type in tables and performing queries.

---

## Directory Structure

- `integration_tests/`: SQL scripts for integration testing.
- `test_pg_iban.c`: Unit test implementation.
- `Dockerfile`: Docker setup for the build unit-testing environment.
- `Dockerfile.integration`: Docker setup for the build integration testing environment.

---

## Contribution

This is an experimental extension. Contributions and improvements are welcome. Please fork the repository and submit a pull request.

---

## License

This project is licensed under the [Apache 2.0](LICENSE).
