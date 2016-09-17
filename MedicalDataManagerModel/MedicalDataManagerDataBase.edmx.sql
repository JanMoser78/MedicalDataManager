
-- --------------------------------------------------
-- Entity Designer DDL Script for SQL Server 2005, 2008, 2012 and Azure
-- --------------------------------------------------
-- Date Created: 09/17/2016 11:47:48
-- Generated from EDMX file: \\mac\home\documents\visual studio 2015\Projects\MedicalDataManager\MedicalDataManagerModel\MedicalDataManagerDataBase.edmx
-- --------------------------------------------------

SET QUOTED_IDENTIFIER OFF;

GO
IF SCHEMA_ID(N'dbo') IS NULL EXECUTE(N'CREATE SCHEMA [dbo]');
GO

-- --------------------------------------------------
-- Dropping existing FOREIGN KEY constraints
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[FK_ActionActionType]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Actions] DROP CONSTRAINT [FK_ActionActionType];
GO
IF OBJECT_ID(N'[dbo].[FK_ActionDevice]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Actions] DROP CONSTRAINT [FK_ActionDevice];
GO
IF OBJECT_ID(N'[dbo].[FK_DeviceUsesTokenDevice]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[DeviceUsesTokens] DROP CONSTRAINT [FK_DeviceUsesTokenDevice];
GO
IF OBJECT_ID(N'[dbo].[FK_DosageContainsSubstancesDosage]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[DosageContainsSubstances] DROP CONSTRAINT [FK_DosageContainsSubstancesDosage];
GO
IF OBJECT_ID(N'[dbo].[FK_DosageContainsSubstancesSubstance]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[DosageContainsSubstances] DROP CONSTRAINT [FK_DosageContainsSubstancesSubstance];
GO
IF OBJECT_ID(N'[dbo].[FK_MedicationPlanDevice]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[MedicationPlans] DROP CONSTRAINT [FK_MedicationPlanDevice];
GO
IF OBJECT_ID(N'[dbo].[FK_MedicationPlanDosage]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[MedicationPlans] DROP CONSTRAINT [FK_MedicationPlanDosage];
GO
IF OBJECT_ID(N'[dbo].[FK_MedicationPlanToken]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[MedicationPlans] DROP CONSTRAINT [FK_MedicationPlanToken];
GO
IF OBJECT_ID(N'[dbo].[FK_TokenAction]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Actions] DROP CONSTRAINT [FK_TokenAction];
GO
IF OBJECT_ID(N'[dbo].[FK_TokenDeviceUsesToken]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[DeviceUsesTokens] DROP CONSTRAINT [FK_TokenDeviceUsesToken];
GO
IF OBJECT_ID(N'[dbo].[FK_UserPerson]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Users] DROP CONSTRAINT [FK_UserPerson];
GO

-- --------------------------------------------------
-- Dropping existing tables
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[Actions]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Actions];
GO
IF OBJECT_ID(N'[dbo].[ActionTypes]', 'U') IS NOT NULL
    DROP TABLE [dbo].[ActionTypes];
GO
IF OBJECT_ID(N'[dbo].[Devices]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Devices];
GO
IF OBJECT_ID(N'[dbo].[DeviceUsesTokens]', 'U') IS NOT NULL
    DROP TABLE [dbo].[DeviceUsesTokens];
GO
IF OBJECT_ID(N'[dbo].[DosageContainsSubstances]', 'U') IS NOT NULL
    DROP TABLE [dbo].[DosageContainsSubstances];
GO
IF OBJECT_ID(N'[dbo].[Dosages]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Dosages];
GO
IF OBJECT_ID(N'[dbo].[MedicationPlans]', 'U') IS NOT NULL
    DROP TABLE [dbo].[MedicationPlans];
GO
IF OBJECT_ID(N'[dbo].[People]', 'U') IS NOT NULL
    DROP TABLE [dbo].[People];
GO
IF OBJECT_ID(N'[dbo].[Substances]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Substances];
GO
IF OBJECT_ID(N'[dbo].[Tokens]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Tokens];
GO
IF OBJECT_ID(N'[dbo].[Users]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Users];
GO

-- --------------------------------------------------
-- Creating all tables
-- --------------------------------------------------

-- Creating table 'People'
CREATE TABLE [dbo].[People] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Firstname] nvarchar(max)  NOT NULL,
    [Lastname] nvarchar(max)  NOT NULL,
    [PhoneNumber] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Devices'
CREATE TABLE [dbo].[Devices] (
    [Id] uniqueidentifier  NOT NULL
);
GO

-- Creating table 'Tokens'
CREATE TABLE [dbo].[Tokens] (
    [Id] int  NOT NULL,
    [Person_Id] int  NOT NULL
);
GO

-- Creating table 'DeviceUsesTokens'
CREATE TABLE [dbo].[DeviceUsesTokens] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [ReadOnly] bit  NOT NULL,
    [DeviceId] uniqueidentifier  NOT NULL,
    [TokenId] int  NOT NULL
);
GO

-- Creating table 'Users'
CREATE TABLE [dbo].[Users] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [UserId] nvarchar(max)  NOT NULL,
    [PassHash] nvarchar(max)  NOT NULL,
    [Person_Id] int  NOT NULL
);
GO

-- Creating table 'ActionTypes'
CREATE TABLE [dbo].[ActionTypes] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Description] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Actions'
CREATE TABLE [dbo].[Actions] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [ActionTypeId] int  NOT NULL,
    [DeviceId] uniqueidentifier  NOT NULL,
    [TimeStamp] datetime  NOT NULL,
    [Payload] nvarchar(max)  NULL,
    [TokenId] int  NOT NULL
);
GO

-- Creating table 'Substances'
CREATE TABLE [dbo].[Substances] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [SubstanceName] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'Dosages'
CREATE TABLE [dbo].[Dosages] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [DosageName] nvarchar(max)  NOT NULL
);
GO

-- Creating table 'DosageContainsSubstances'
CREATE TABLE [dbo].[DosageContainsSubstances] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [DosageId] int  NOT NULL,
    [SubstanceId] int  NOT NULL
);
GO

-- Creating table 'MedicationPlans'
CREATE TABLE [dbo].[MedicationPlans] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [DeviceId] uniqueidentifier  NOT NULL,
    [Critical] bit  NOT NULL,
    [DosageId] int  NOT NULL,
    [ValidFrom] nvarchar(max)  NOT NULL,
    [Validto] nvarchar(max)  NOT NULL,
    [CompartmentNbr] int  NOT NULL,
    [TokenId] int  NOT NULL
);
GO

-- --------------------------------------------------
-- Creating all PRIMARY KEY constraints
-- --------------------------------------------------

-- Creating primary key on [Id] in table 'People'
ALTER TABLE [dbo].[People]
ADD CONSTRAINT [PK_People]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Devices'
ALTER TABLE [dbo].[Devices]
ADD CONSTRAINT [PK_Devices]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Tokens'
ALTER TABLE [dbo].[Tokens]
ADD CONSTRAINT [PK_Tokens]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'DeviceUsesTokens'
ALTER TABLE [dbo].[DeviceUsesTokens]
ADD CONSTRAINT [PK_DeviceUsesTokens]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Users'
ALTER TABLE [dbo].[Users]
ADD CONSTRAINT [PK_Users]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'ActionTypes'
ALTER TABLE [dbo].[ActionTypes]
ADD CONSTRAINT [PK_ActionTypes]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [PK_Actions]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Substances'
ALTER TABLE [dbo].[Substances]
ADD CONSTRAINT [PK_Substances]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Dosages'
ALTER TABLE [dbo].[Dosages]
ADD CONSTRAINT [PK_Dosages]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'DosageContainsSubstances'
ALTER TABLE [dbo].[DosageContainsSubstances]
ADD CONSTRAINT [PK_DosageContainsSubstances]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'MedicationPlans'
ALTER TABLE [dbo].[MedicationPlans]
ADD CONSTRAINT [PK_MedicationPlans]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- --------------------------------------------------
-- Creating all FOREIGN KEY constraints
-- --------------------------------------------------

-- Creating foreign key on [Person_Id] in table 'Users'
ALTER TABLE [dbo].[Users]
ADD CONSTRAINT [FK_UserPerson]
    FOREIGN KEY ([Person_Id])
    REFERENCES [dbo].[People]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_UserPerson'
CREATE INDEX [IX_FK_UserPerson]
ON [dbo].[Users]
    ([Person_Id]);
GO

-- Creating foreign key on [ActionTypeId] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [FK_ActionActionType]
    FOREIGN KEY ([ActionTypeId])
    REFERENCES [dbo].[ActionTypes]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_ActionActionType'
CREATE INDEX [IX_FK_ActionActionType]
ON [dbo].[Actions]
    ([ActionTypeId]);
GO

-- Creating foreign key on [DeviceId] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [FK_ActionDevice]
    FOREIGN KEY ([DeviceId])
    REFERENCES [dbo].[Devices]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_ActionDevice'
CREATE INDEX [IX_FK_ActionDevice]
ON [dbo].[Actions]
    ([DeviceId]);
GO

-- Creating foreign key on [DeviceId] in table 'DeviceUsesTokens'
ALTER TABLE [dbo].[DeviceUsesTokens]
ADD CONSTRAINT [FK_DeviceUsesTokenDevice]
    FOREIGN KEY ([DeviceId])
    REFERENCES [dbo].[Devices]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_DeviceUsesTokenDevice'
CREATE INDEX [IX_FK_DeviceUsesTokenDevice]
ON [dbo].[DeviceUsesTokens]
    ([DeviceId]);
GO

-- Creating foreign key on [DosageId] in table 'DosageContainsSubstances'
ALTER TABLE [dbo].[DosageContainsSubstances]
ADD CONSTRAINT [FK_DosageContainsSubstancesDosage]
    FOREIGN KEY ([DosageId])
    REFERENCES [dbo].[Dosages]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_DosageContainsSubstancesDosage'
CREATE INDEX [IX_FK_DosageContainsSubstancesDosage]
ON [dbo].[DosageContainsSubstances]
    ([DosageId]);
GO

-- Creating foreign key on [SubstanceId] in table 'DosageContainsSubstances'
ALTER TABLE [dbo].[DosageContainsSubstances]
ADD CONSTRAINT [FK_DosageContainsSubstancesSubstance]
    FOREIGN KEY ([SubstanceId])
    REFERENCES [dbo].[Substances]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_DosageContainsSubstancesSubstance'
CREATE INDEX [IX_FK_DosageContainsSubstancesSubstance]
ON [dbo].[DosageContainsSubstances]
    ([SubstanceId]);
GO

-- Creating foreign key on [DeviceId] in table 'MedicationPlans'
ALTER TABLE [dbo].[MedicationPlans]
ADD CONSTRAINT [FK_MedicationPlanDevice]
    FOREIGN KEY ([DeviceId])
    REFERENCES [dbo].[Devices]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_MedicationPlanDevice'
CREATE INDEX [IX_FK_MedicationPlanDevice]
ON [dbo].[MedicationPlans]
    ([DeviceId]);
GO

-- Creating foreign key on [DosageId] in table 'MedicationPlans'
ALTER TABLE [dbo].[MedicationPlans]
ADD CONSTRAINT [FK_MedicationPlanDosage]
    FOREIGN KEY ([DosageId])
    REFERENCES [dbo].[Dosages]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_MedicationPlanDosage'
CREATE INDEX [IX_FK_MedicationPlanDosage]
ON [dbo].[MedicationPlans]
    ([DosageId]);
GO

-- Creating foreign key on [TokenId] in table 'DeviceUsesTokens'
ALTER TABLE [dbo].[DeviceUsesTokens]
ADD CONSTRAINT [FK_TokenDeviceUsesToken]
    FOREIGN KEY ([TokenId])
    REFERENCES [dbo].[Tokens]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_TokenDeviceUsesToken'
CREATE INDEX [IX_FK_TokenDeviceUsesToken]
ON [dbo].[DeviceUsesTokens]
    ([TokenId]);
GO

-- Creating foreign key on [TokenId] in table 'Actions'
ALTER TABLE [dbo].[Actions]
ADD CONSTRAINT [FK_TokenAction]
    FOREIGN KEY ([TokenId])
    REFERENCES [dbo].[Tokens]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_TokenAction'
CREATE INDEX [IX_FK_TokenAction]
ON [dbo].[Actions]
    ([TokenId]);
GO

-- Creating foreign key on [TokenId] in table 'MedicationPlans'
ALTER TABLE [dbo].[MedicationPlans]
ADD CONSTRAINT [FK_MedicationPlanToken]
    FOREIGN KEY ([TokenId])
    REFERENCES [dbo].[Tokens]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;
GO

-- Creating non-clustered index for FOREIGN KEY 'FK_MedicationPlanToken'
CREATE INDEX [IX_FK_MedicationPlanToken]
ON [dbo].[MedicationPlans]
    ([TokenId]);
GO

-- --------------------------------------------------
-- Script has ended
-- --------------------------------------------------